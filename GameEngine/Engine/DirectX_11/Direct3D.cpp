#include <d3dcompiler.h>
#include"../../EngineTime.h"
#include "Direct3D.h"
#include"../../DebugUI.h"
#include<array>

//変数
namespace Direct3D
{
	ID3D11Device*			 pDevice;										//デバイス
	ID3D11DeviceContext*	 pContext;										//デバイスコンテキスト
	IDXGISwapChain*			 pSwapChain;									//スワップチェイン
	ID3D11RenderTargetView*	 pRenderTargetView;								//レンダーターゲットビュー
	ID3D11Texture2D*		 pDepthStencil;									//深度ステンシル
	ID3D11DepthStencilView*	 pDepthStencilView;								//深度ステンシルビュー
	ID3D11BlendState*		 pBlendState[(int)BLEND_MODE::BLEND_MAX];		//ブレンドステート
	ID3D11DepthStencilState* pDepthStencilState[(int)BLEND_MODE::BLEND_MAX];//デプスステンシルステート


	//深度テクスチャ用
	ID3D11Texture2D*		  pDepthTexture;			//描画先
	ID3D11RenderTargetView*	  pDepthRenderTargetView;	//レンダーターゲット
	ID3D11DepthStencilView*	  pDepthDepthStencilView;	//ステンシルバッファ
	ID3D11Texture2D*		  pDepthDepthStencil;		//深度バッファ用テクスチャ
	ID3D11ShaderResourceView* pDepthSRV;		//深度テクスチャをシェーダーに渡すやつ
	D3D11_VIEWPORT depthVp;

	struct SHADER_BUNDLE
	{
		ID3D11VertexShader* pVertexShader = nullptr;	//頂点シェーダー
		ID3D11PixelShader* pPixelShader = nullptr;		//ピクセルシェーダー
		ID3D11InputLayout* pVertexLayout = nullptr;		//頂点インプットレイアウト
		ID3D11RasterizerState* pRasterizerState = nullptr;	//ラスタライザー
	};
	SHADER_BUNDLE shaderBundle[(int)SHADER_TYPE::SHADER_MAX];
	D3D11_VIEWPORT vp;
	int screenWidth;
	int screenHeight;
	XMMATRIX clipToUVMatrix;
	XMMATRIX lightViewMatrix;
	XMMATRIX lightPrjMatrix_;
	ID3D11SamplerState* pDepthSampler;
	bool useShadow = true;
	bool shadowRender;

}

//初期化
HRESULT Direct3D::Initialize(int winW, int winH, HWND hWnd)
{

	screenWidth = winW;
	screenHeight = winH;
	///////////////////////////いろいろ準備するための設定///////////////////////////////
	//いろいろな設定項目をまとめた構造体
	DXGI_SWAP_CHAIN_DESC scDesc;

	//とりあえず全部0
	ZeroMemory(&scDesc, sizeof(scDesc));

	//描画先のフォーマット
	scDesc.BufferDesc.Width = winW;		//画面幅
	scDesc.BufferDesc.Height = winH;	//画面高さ
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 何色使えるか

	//FPS（1/60秒に1回）
	scDesc.BufferDesc.RefreshRate.Numerator = 60;
	scDesc.BufferDesc.RefreshRate.Denominator = 1;

	//その他
	scDesc.Windowed = TRUE;			//ウィンドウモードかフルスクリーンか
	scDesc.OutputWindow = hWnd;		//ウィンドウハンドル
	scDesc.BufferCount = 1;			//バックバッファの枚数
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//バックバッファの使い道＝画面に描画するために
	scDesc.SampleDesc.Count = 1;		//MSAA（アンチエイリアス）の設定
	scDesc.SampleDesc.Quality = 0;		//　〃

	////////////////上記設定をもとにデバイス、コンテキスト、スワップチェインを作成////////////////////////
	D3D_FEATURE_LEVEL level;
	D3D11CreateDeviceAndSwapChain(
		nullptr,						// どのビデオアダプタを使用するか？既定ならばnullptrで
		D3D_DRIVER_TYPE_HARDWARE,		// ドライバのタイプを渡す。ふつうはHARDWARE
		nullptr,						// 上記をD3D_DRIVER_TYPE_SOFTWAREに設定しないかぎりnullptr
		D3D10_CREATE_DEVICE_BGRA_SUPPORT,//D3D11_CREATE_DEVICE_DISABLE_GPU_TIMEOUT,								// 何らかのフラグを指定する。（デバッグ時はD3D11_CREATE_DEVICE_DEBUG？）
		nullptr,						// デバイス、コンテキストのレベルを設定。nullptrにしとけばOK
		0,								// 上の引数でレベルを何個指定したか
		D3D11_SDK_VERSION,				// SDKのバージョン。必ずこの値
		&scDesc,						// 上でいろいろ設定した構造体
		&pSwapChain,					// 無事完成したSwapChainのアドレスが返ってくる
		&pDevice,						// 無事完成したDeviceアドレスが返ってくる
		&level,							// 無事完成したDevice、Contextのレベルが返ってくる
		&pContext);						// 無事完成したContextのアドレスが返ってくる

	///////////////////////////レンダーターゲットビュー作成///////////////////////////////
	//スワップチェーンからバックバッファを取得（バックバッファ ＝ レンダーターゲット）

	//レンダーターゲットビューを作成
	ID3D11Texture2D* pBackBuffer;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);

	//一時的にバックバッファを取得しただけなので解放
	SAFE_RELEASE(pBackBuffer);

	///////////////////////////ビューポート（描画範囲）設定///////////////////////////////
	//レンダリング結果を表示する範囲
	vp;
	vp.Width = (float)winW;	//幅
	vp.Height = (float)winH;//高さ
	vp.MinDepth = 0.0f;	//手前
	vp.MaxDepth = 1.0f;	//奥
	vp.TopLeftX = 0;	//左
	vp.TopLeftY = 0;	//上


	//深度ステンシルビューの作成
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = winW;
	descDepth.Height = winH;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	pDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
	pDevice->CreateDepthStencilView(pDepthStencil, NULL, &pDepthStencilView);


	//深度ステンシルステートの作成
	D3D11_DEPTH_STENCIL_DESC depthstencildesc = {};
	depthstencildesc.DepthEnable = true;
	depthstencildesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthstencildesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthstencildesc.StencilEnable = true;

	pDevice->CreateDepthStencilState(&depthstencildesc, &pDepthStencilState[(int)BLEND_MODE::BLEND_DEFAULT]);
	pContext->OMSetDepthStencilState(pDepthStencilState[(int)BLEND_MODE::BLEND_DEFAULT], 0);

	depthstencildesc.StencilEnable = false;
	pDevice->CreateDepthStencilState(&depthstencildesc, &pDepthStencilState[(int)BLEND_MODE::BLEND_ADD]);

	//ブレンドステートの作成

	//デフォルト
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	float factor[4] = { D3D11_BLEND_ZERO,D3D11_BLEND_ZERO ,D3D11_BLEND_ZERO ,D3D11_BLEND_ZERO };
	HRESULT hr;
	hr=pDevice->CreateBlendState(&desc, &pBlendState[(int)BLEND_MODE::BLEND_DEFAULT]);

	if(FAILED(hr))
	{
		MessageBox(nullptr, L"ブレンドステートの作成に失敗", L"エラー", MB_OK);
		return hr;
	}
	pContext->OMSetBlendState(pBlendState[(int)BLEND_MODE::BLEND_DEFAULT], factor, 0xffffffff);
	//加算合成
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	pDevice->CreateBlendState(&desc, &pBlendState[(int)BLEND_MODE::BLEND_ADD]);



	//データを画面に描画するための一通りの設定（パイプライン）
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);       //データの入力種類を指定
	pContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);        // 描画先を設定
	pContext->RSSetViewports(1, &vp);


	hr=InitDepthTexture();
	if (FAILED(hr))
		return hr;

	//シェーダー準備
	 hr=InitShader();
	 if (FAILED(hr))
	 {
		 return hr;
	 }
	 useShadow = false;
	

	 return S_OK;
}

HRESULT Direct3D::InitDepthTexture()
{
	HRESULT hr;

	//深度テクスチャの描画先作成
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = screenWidth;
	textureDesc.Height = screenHeight;
	textureDesc.ArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.MiscFlags = 0;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	hr=pDevice->CreateTexture2D(&textureDesc, NULL, &pDepthTexture);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"深度テクスチャ作成に失敗", L"エラー", MB_OK);
		return hr;
	}

	//レンダーターゲットビュー作成
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	hr = pDevice->CreateRenderTargetView(pDepthTexture, &rtvDesc, &pDepthRenderTargetView);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"深度テクスチャ用レンダーターゲットビュー作成に失敗", L"エラー", MB_OK);
		return hr;
	}

	D3D11_TEXTURE2D_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(D3D11_TEXTURE2D_DESC));
	depthDesc.Width = screenWidth;
	depthDesc.Height = screenHeight;
	depthDesc.ArraySize = 1;
	depthDesc.MipLevels = 1;
	depthDesc.MiscFlags = 0;
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;

	hr = pDevice->CreateTexture2D(&depthDesc, NULL, &pDepthDepthStencil);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"深度テクスチャ用デプスステンシル作成に失敗", L"エラー", MB_OK);
		return hr;
	}

	hr = pDevice->CreateDepthStencilView(pDepthDepthStencil, NULL, &pDepthDepthStencilView);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"深度テクスチャ用デプスステンシルビュー作成に失敗", L"エラー", MB_OK);
		return hr;
	}

	//深度テクスチャ用シェーダーリソースビュー作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	hr = pDevice->CreateShaderResourceView(pDepthTexture, &srvDesc, &pDepthSRV);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"深度テクスチャ用シェーダーリソースビュー作成に失敗", L"エラー", MB_OK);
		return hr;
	}

	XMFLOAT4X4 clip;
	ZeroMemory(&clip, sizeof(XMFLOAT4X4));
	clip = { 0.5,     0,   0,   0,
			   0,  -0.5,   0,   0,
			   0,     0, 1.0,   0,
			 0.5,   0.5,   0, 1.0 };
	/*clip._11 = 0.5f;
	clip._22 = -0.5f;
	clip._33 = 1.0f;
	clip._41 = 0.5f;
	clip._42 = 0.5f;
	clip._44 = 1.0f;*/
	clipToUVMatrix = XMLoadFloat4x4(&clip);

	depthVp.Width = (float)screenWidth;	//幅
	depthVp.Height = (float)screenHeight;//高さ
	depthVp.MinDepth = 0.0f;	//手前
	depthVp.MaxDepth = 1.0f;	//奥
	depthVp.TopLeftX = 0;	//左
	depthVp.TopLeftY = 0;	//上

	//深度テクスチャ用サンプラー作成
	D3D11_SAMPLER_DESC  depthSmplDesc;
	ZeroMemory(&depthSmplDesc, sizeof(D3D11_SAMPLER_DESC));
	depthSmplDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	depthSmplDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	depthSmplDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	depthSmplDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	lightViewMatrix = XMMatrixLookAtLH(XMVectorSet(50, 760, -50, 0),
		XMVectorSet(0, 0,0, 0),
		XMVectorSet(0, 1, 0, 0));
	lightPrjMatrix_= XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)screenWidth / (FLOAT)screenHeight,330, 800);

	hr = Direct3D::pDevice->CreateSamplerState(&depthSmplDesc, &pDepthSampler);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Texture:サンプラー作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}
	return TRUE;
}

//シェーダー準備
HRESULT Direct3D::InitShader()
{
	if (FAILED(InitShader2D()))
		return E_FAIL;

	if (FAILED(InitShader3D()))
		return E_FAIL;

	if (FAILED(InitShaderEFF()))
		return E_FAIL;

	if (FAILED(InitShaderOutLine()))
		return E_FAIL;

	if (FAILED(InitShaderToon()))
		return E_FAIL;
	
	if (FAILED(InitShaderShadow()))
		return E_FAIL;
	
	return S_OK;

}

HRESULT Direct3D::InitShader2D()
{
	HRESULT hr;
	// 頂点シェーダの作成（コンパイル）
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"Simple2D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL,
									 &shaderBundle[(int)SHADER_TYPE::SHADER_2D].pVertexShader);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pCompileVS);
		MessageBox(nullptr, L"頂点シェーダの作成に失敗", L"エラー", MB_OK);
		return hr;
	}

	//頂点インプットレイアウト   頂点シェーダ―に渡す情報
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//位置
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMVECTOR),  D3D11_INPUT_PER_VERTEX_DATA, 0 },  //UV座標
	};
	hr = pDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), pCompileVS->GetBufferPointer(),
									pCompileVS->GetBufferSize(), &shaderBundle[(int)SHADER_TYPE::SHADER_2D].pVertexLayout);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pCompileVS);
		MessageBox(nullptr, L"頂点インプットレイアウトの設定に失敗", L"エラー", MB_OK);
		return hr;
	}
	SAFE_RELEASE(pCompileVS);

	// ピクセルシェーダの作成（コンパイル）
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"Simple2D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	assert(pCompilePS != nullptr);
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL,
									&shaderBundle[(int)SHADER_TYPE::SHADER_2D].pPixelShader);

	if (FAILED(hr))
	{
		SAFE_RELEASE(pCompilePS);
		MessageBox(nullptr, L"ピクセルシェーダの作成に失敗", L"エラー", MB_OK);
		return hr;
	}
	SAFE_RELEASE(pCompilePS);

	//ラスタライザ作成
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE;
	hr = pDevice->CreateRasterizerState(&rdc, &shaderBundle[(int)SHADER_TYPE::SHADER_2D].pRasterizerState);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pCompilePS);
		MessageBox(nullptr, L"ピクセルシェーダの作成に失敗", L"エラー", MB_OK);
		return hr;
	}

	return S_OK;
}

HRESULT Direct3D::InitShader3D()
{
	HRESULT hr;
	// 頂点シェーダの作成（コンパイル）
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"newSimple3D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL,
									 &shaderBundle[(int)SHADER_TYPE::SHADER_3D].pVertexShader);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pCompileVS);
		MessageBox(nullptr, L"頂点シェーダの作成に失敗", L"エラー", MB_OK);
		return hr;
	}

	//////////////////////////////////////////////////頂点インプットレイアウト///////////////////////////////////////////////
	//HLSL(シェーダーのソースコード)に送る情報の種類とその設定を行う
	//1.セマンティックの名前
	//2.セマンティックインデックス(同じセマンティックを持つ要素が複数あるときに使う)
	//3.要素データのデータデータ型
	//4.入力アセンブラを識別する整数値(0〜15)
	//5.頂点の先頭アドレスからのオフセット(バイト単位)
	//6.入力スロットの入力データクラスの識別
	//7.バッファ内で1要素進む前に、同じインスタンスごとのデータを使用して
	//描画するインスタンスの数頂点単位のデータを含む要素の場合、0にする・・・らしい
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,				0,	   D3D11_INPUT_PER_VERTEX_DATA, 0 },  //位置
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, sizeof(XMVECTOR),	   D3D11_INPUT_PER_VERTEX_DATA, 0 },  //UV座標
		{ "NORMAL",	  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 2, D3D11_INPUT_PER_VERTEX_DATA, 0 },  //法線
		{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },  //接線
	};

	hr = pDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), pCompileVS->GetBufferPointer(), 
									pCompileVS->GetBufferSize(), &shaderBundle[(int)SHADER_TYPE::SHADER_3D].pVertexLayout);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pCompileVS);
		MessageBox(nullptr, L"頂点インプットレイアウトの設定に失敗", L"エラー", MB_OK);
		return hr;
	}
	SAFE_RELEASE(pCompileVS);

	// ピクセルシェーダの作成（コンパイル）
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"newSimple3D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	assert(pCompilePS != nullptr);
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL,
									&shaderBundle[(int)SHADER_TYPE::SHADER_3D].pPixelShader);
	SAFE_RELEASE(pCompilePS);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"ピクセルシェーダの作成に失敗", L"エラー", MB_OK);
		return hr;
	}

	//ラスタライザ作成
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE;
	hr = pDevice->CreateRasterizerState(&rdc, &shaderBundle[(int)SHADER_TYPE::SHADER_3D].pRasterizerState);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"ラスタライザの作成に失敗", L"エラー", MB_OK);
		return hr;
	}

	return S_OK;
}

HRESULT Direct3D::InitShaderEFF()
{
	HRESULT hr;
	// 頂点シェーダの作成（コンパイル）
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"BillBoard.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL,
									 &shaderBundle[(int)SHADER_TYPE::SHADER_EFF].pVertexShader);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pCompileVS);
		MessageBox(nullptr, L"頂点シェーダの作成に失敗", L"エラー", MB_OK);
		return hr;
	}

	// ピクセルシェーダの作成（コンパイル）
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"BillBoard.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	assert(pCompilePS != nullptr);
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL,
									&shaderBundle[(int)SHADER_TYPE::SHADER_EFF].pPixelShader);
	SAFE_RELEASE(pCompilePS);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"ピクセルシェーダの作成に失敗", L"エラー", MB_OK);
		return hr;
	}

	//////////////////////////////////////////////////頂点インプットレイアウト///////////////////////////////////////////////
	//HLSL(シェーダーの事)に送る情報の種類とその設定を行う
	//1.セマンティックの名前
	//2.セマンティックインデックス(同じセマンティックを持つ要素が複数あるときに使う)
	//3.要素データのデータデータ型
	//4.入力アセンブラを識別する整数値(0〜15)
	//5.頂点の先頭アドレスからのオフセット(バイト単位)
	//6.入力スロットの入力データクラスの識別
	//7.バッファ内で1要素進む前に、同じインスタンスごとのデータを使用して
	//描画するインスタンスの数頂点単位のデータを含む要素の場合、0にする・・・らしい
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,					D3D11_INPUT_PER_VERTEX_DATA, 0 },  //位置
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	  0, sizeof(XMFLOAT3),  D3D11_INPUT_PER_VERTEX_DATA, 0 },  //UV座標
	};

	hr = pDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), pCompileVS->GetBufferPointer(),
									pCompileVS->GetBufferSize(), &shaderBundle[(int)SHADER_TYPE::SHADER_EFF].pVertexLayout);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pCompileVS);
		MessageBox(nullptr, L"頂点インプットレイアウトの設定に失敗", L"エラー", MB_OK);
		return hr;
	}
	SAFE_RELEASE(pCompileVS);


	//ラスタライザ作成
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_NONE;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise =TRUE;
	hr = pDevice->CreateRasterizerState(&rdc, &shaderBundle[(int)SHADER_TYPE::SHADER_EFF].pRasterizerState);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"ラスタライザの作成に失敗", L"エラー", MB_OK);
		return hr;
	}

	return S_OK;
}

HRESULT Direct3D::InitShaderOutLine()
{
	HRESULT hr;
	// 頂点シェーダの作成（コンパイル）
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"OutLine.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, 
									 &shaderBundle[(int)SHADER_TYPE::SHADER_OUTLINE].pVertexShader);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pCompileVS);
		MessageBox(nullptr, L"頂点シェーダの作成に失敗", L"エラー", MB_OK);
		return hr;
	}

	//////////////////////////////////////////////////頂点インプットレイアウト///////////////////////////////////////////////
	//HLSL(シェーダーの事)に送る情報の種類とその設定を行う
	//1.セマンティックの名前
	//2.セマンティックインデックス(同じセマンティックを持つ要素が複数あるときに使う)
	//3.要素データのデータデータ型
	//4.入力アセンブラを識別する整数値(0〜15)
	//5.頂点の先頭アドレスからのオフセット(バイト単位)
	//6.入力スロットの入力データクラスの識別
	//7.バッファ内で1要素進む前に、同じインスタンスごとのデータを使用して
	//描画するインスタンスの数頂点単位のデータを含む要素の場合、0にする・・・らしい
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,				0,	   D3D11_INPUT_PER_VERTEX_DATA, 0 },  //位置
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, sizeof(XMVECTOR),	   D3D11_INPUT_PER_VERTEX_DATA, 0 },  //UV座標
		{ "NORMAL",	  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 2, D3D11_INPUT_PER_VERTEX_DATA, 0 },  //法線
		{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },  //接線
	};

	hr = pDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), pCompileVS->GetBufferPointer(),
									pCompileVS->GetBufferSize(), &shaderBundle[(int)SHADER_TYPE::SHADER_OUTLINE].pVertexLayout);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pCompileVS);
		MessageBox(nullptr, L"頂点インプットレイアウトの設定に失敗", L"エラー", MB_OK);
		return hr;
	}
	SAFE_RELEASE(pCompileVS);

	// ピクセルシェーダの作成（コンパイル）
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"OutLine.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	assert(pCompilePS != nullptr);
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL,
									&shaderBundle[(int)SHADER_TYPE::SHADER_OUTLINE].pPixelShader);
	SAFE_RELEASE(pCompilePS);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"ピクセルシェーダの作成に失敗", L"エラー", MB_OK);
		return hr;
	}

	//ラスタライザ作成
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_FRONT;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE;
	hr = pDevice->CreateRasterizerState(&rdc, &shaderBundle[(int)SHADER_TYPE::SHADER_OUTLINE].pRasterizerState);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"ラスタライザの作成に失敗", L"エラー", MB_OK);
		return hr;
	}

	return S_OK;
}

HRESULT Direct3D::InitShaderToon()
{
	HRESULT hr;
	// 頂点シェーダの作成（コンパイル）
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"ToonShader.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL,
									 &shaderBundle[(int)SHADER_TYPE::SHADER_TOON].pVertexShader);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pCompileVS);
		MessageBox(nullptr, L"頂点シェーダの作成に失敗", L"エラー", MB_OK);
		return hr;
	}

	//////////////////////////////////////////////////頂点インプットレイアウト///////////////////////////////////////////////
	//HLSL(シェーダーの事)に送る情報の種類とその設定を行う
	//1.セマンティックの名前
	//2.セマンティックインデックス(同じセマンティックを持つ要素が複数あるときに使う)
	//3.要素データのデータデータ型
	//4.入力アセンブラを識別する整数値(0〜15)
	//5.頂点の先頭アドレスからのオフセット(バイト単位)
	//6.入力スロットの入力データクラスの識別
	//7.バッファ内で1要素進む前に、同じインスタンスごとのデータを使用して
	//描画するインスタンスの数頂点単位のデータを含む要素の場合、0にする・・・らしい
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,				0,	   D3D11_INPUT_PER_VERTEX_DATA, 0 },  //位置
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, sizeof(XMVECTOR),	   D3D11_INPUT_PER_VERTEX_DATA, 0 },  //UV座標
		{ "NORMAL",	  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 2, D3D11_INPUT_PER_VERTEX_DATA, 0 },  //法線
		{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },  //接線
	};

	hr = pDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), 
									&shaderBundle[(int)SHADER_TYPE::SHADER_TOON].pVertexLayout);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pCompileVS);
		MessageBox(nullptr, L"頂点インプットレイアウトの設定に失敗", L"エラー", MB_OK);
		return hr;
	}
	SAFE_RELEASE(pCompileVS);

	// ピクセルシェーダの作成（コンパイル）
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"ToonShader.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	assert(pCompilePS != nullptr);
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, 
									&shaderBundle[(int)SHADER_TYPE::SHADER_TOON].pPixelShader);
	SAFE_RELEASE(pCompilePS);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"ピクセルシェーダの作成に失敗", L"エラー", MB_OK);
		return hr;
	}

	//ラスタライザ作成
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE;
	hr = pDevice->CreateRasterizerState(&rdc, &shaderBundle[(int)SHADER_TYPE::SHADER_TOON].pRasterizerState);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"ラスタライザの作成に失敗", L"エラー", MB_OK);
		return hr;
	}

	return S_OK;
}

HRESULT Direct3D::InitShaderShadow()
{
	HRESULT hr;
	// 頂点シェーダの作成（コンパイル）
	ID3DBlob* pCompileVS = nullptr;
	ID3DBlob* pVSError = nullptr;
	hr=D3DCompileFromFile(L"DepthShader.hlsl", nullptr, nullptr, "VS_Depth", "vs_5_0", NULL, 0, &pCompileVS, &pVSError);
	assert(pCompileVS != nullptr);
	if (FAILED(hr))
	{
		if (pVSError != nullptr)
		{
			MessageBox(nullptr, (LPCWSTR)pVSError->GetBufferPointer(), L"エラー", MB_OK);
		}

		SAFE_RELEASE(pVSError);
		SAFE_RELEASE(pCompileVS);
		MessageBox(nullptr, L"頂点シェーダの作成に失敗", L"エラー", MB_OK);
		return hr;
	}
	SAFE_RELEASE(pVSError);
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL,
		&shaderBundle[(int)SHADER_TYPE::SHADER_DEPTH].pVertexShader);

	//////////////////////////////////////////////////頂点インプットレイアウト///////////////////////////////////////////////
	//HLSL(シェーダーのソースコード)に送る情報の種類とその設定を行う
	//1.セマンティックの名前
	//2.セマンティックインデックス(同じセマンティックを持つ要素が複数あるときに使う)
	//3.要素データのデータデータ型
	//4.入力アセンブラを識別する整数値(0〜15)
	//5.頂点の先頭アドレスからのオフセット(バイト単位)
	//6.入力スロットの入力データクラスの識別
	//7.バッファ内で1要素進む前に、同じインスタンスごとのデータを使用して
	//描画するインスタンスの数頂点単位のデータを含む要素の場合、0にする・・・らしい
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },  //位置
	};

	hr = pDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), pCompileVS->GetBufferPointer(),
		pCompileVS->GetBufferSize(), &shaderBundle[(int)SHADER_TYPE::SHADER_DEPTH].pVertexLayout);
	if (FAILED(hr))
	{
		
		SAFE_RELEASE(pCompileVS);
		MessageBox(nullptr, L"頂点インプットレイアウトの設定に失敗", L"エラー", MB_OK);
		return hr;
	}
	SAFE_RELEASE(pCompileVS);

	// ピクセルシェーダの作成（コンパイル）
	ID3DBlob* pCompilePS = nullptr;
	ID3DBlob* pPSError = nullptr;
	D3DCompileFromFile(L"DepthShader.hlsl", nullptr, nullptr, "PS_Depth", "ps_5_0", NULL, 0, &pCompilePS, &pPSError);
	assert(pCompilePS != nullptr);
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL,
		&shaderBundle[(int)SHADER_TYPE::SHADER_DEPTH].pPixelShader);

	SAFE_RELEASE(pCompilePS);
	if (FAILED(hr))
	{
		if (pPSError != nullptr)
		{
			MessageBox(nullptr, (LPCWSTR)pPSError->GetBufferPointer(), L"エラー", MB_OK);
		}
		SAFE_RELEASE(pPSError);
		MessageBox(nullptr, L"ピクセルシェーダの作成に失敗", L"エラー", MB_OK);
		return hr;
	}
	SAFE_RELEASE(pPSError);

	//ラスタライザ作成
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE;
	hr = pDevice->CreateRasterizerState(&rdc, &shaderBundle[(int)SHADER_TYPE::SHADER_DEPTH].pRasterizerState);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"ラスタライザの作成に失敗", L"エラー", MB_OK);
		return hr;
	}

	return S_OK;
}

void Direct3D::SetShader(SHADER_TYPE type)
{
	//それぞれをデバイスコンテキストにセット  これらの情報を使って描画を行う
	pContext->VSSetShader(shaderBundle[(int)type].pVertexShader, NULL, 0);	//頂点シェーダー
	pContext->PSSetShader(shaderBundle[(int)type].pPixelShader, NULL, 0);	//ピクセルシェーダー
	pContext->IASetInputLayout(shaderBundle[(int)type].pVertexLayout);	//頂点インプットレイアウト
	pContext->RSSetState(shaderBundle[(int)type].pRasterizerState);		//ラスタライザー
}

void Direct3D::SetBlendMode(BLEND_MODE mode)
{
	float factor[4] = { D3D11_BLEND_ZERO,D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
	Direct3D::pContext->OMSetBlendState(pBlendState[(int)mode], factor, 0xffffffff);			//ブレンドステート

	pContext->OMSetDepthStencilState(pDepthStencilState[(int)mode], 0);
}
void Direct3D::SetDepthRender()
{
	

}
bool Direct3D::IsUseShadow()
{
	return useShadow;
}
void Direct3D::SetShadowEnable(bool isUseShadow)
{
	useShadow = isUseShadow;
}
bool Direct3D::IsRenderShadow()
{
	return shadowRender;
}
ID3D11Device* Direct3D::GetDevice()
{
	return pDevice;
}

ID3D11DeviceContext* Direct3D::GetContext()
{
	 return pContext;
}
IDXGISwapChain* Direct3D::GetSwapChain()
{
	return pSwapChain;
}
ID3D11ShaderResourceView* Direct3D::GetDepthSRV()
{
	return pDepthSRV;
}
int Direct3D::GetScreenWidth()
{
	return screenWidth;
}
int Direct3D::GetScreenHeight()
{
	return screenHeight;
}
void Direct3D::SetDepthBufferWriteEnable(bool isWrite)
{
	if (isWrite)
	{
		pContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
	}

	else
	{
		pContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);
	}
}
void Direct3D::BeginShadowDraw()
{
		float color[4] = { 0,0,0,0 };
	pContext->OMSetRenderTargets(1, &pDepthRenderTargetView, pDepthDepthStencilView);
	pContext->ClearRenderTargetView(pDepthRenderTargetView, color);
	pContext->ClearDepthStencilView(pDepthDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	pContext->RSSetViewports(1,&depthVp);
	shadowRender = true;
}
void Direct3D::EndShadowDraw()
{
	shadowRender = false;
}
//描画開始
void Direct3D::BeginDraw()
{
	EngineTime::SetTime();
	//画面をクリア
		//背景色
	float clearColor[4] = { 0.1f,0.5f,0.5f,1.0f };
	pContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
	//レンダーターゲットビューをクリア
	pContext->ClearRenderTargetView(pRenderTargetView, clearColor);
	//深度バッファクリア
	pContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	pContext->RSSetViewports(1, &vp);
}

//描画終了
void Direct3D::EndDraw()
{
	EngineTime::SetTime();
	//スワップ（バックバッファを表に表示する）
	HRESULT hr = pSwapChain->Present(0, 0);
	if (hr== DXGI_ERROR_DEVICE_REMOVED || hr== DXGI_ERROR_DEVICE_RESET)
	{
		
			MessageBox(nullptr, L"Direct3D : スワップチェイン失敗", L"エラー", MB_OK);
		
	}
}

//解放処理
void Direct3D::Release()
{
	//解放処理
	for (int releaseShader = 0; releaseShader < (int)SHADER_TYPE::SHADER_MAX; releaseShader++)
	{
		SAFE_RELEASE(shaderBundle[releaseShader].pRasterizerState);
		SAFE_RELEASE(shaderBundle[releaseShader].pVertexLayout);
		SAFE_RELEASE(shaderBundle[releaseShader].pPixelShader);
		SAFE_RELEASE(shaderBundle[releaseShader].pVertexShader);
	}

	for (int i = 0; i < (int)BLEND_MODE::BLEND_MAX; i++)
	{
		SAFE_RELEASE(pDepthStencilState[i]);
		SAFE_RELEASE(pBlendState[i]);				//深度ステンシル
	}
	SAFE_RELEASE(pDepthSRV);
	SAFE_RELEASE(pDepthDepthStencil);
	SAFE_RELEASE(pDepthDepthStencilView);
	SAFE_RELEASE(pDepthRenderTargetView);
	SAFE_RELEASE(pDepthTexture);
	SAFE_RELEASE(pDepthStencilView);				//深度ステンシルビュー
	SAFE_RELEASE(pDepthStencil);
	SAFE_RELEASE(pRenderTargetView);
	SAFE_RELEASE(pSwapChain);		//スワップチェイン
	SAFE_RELEASE(pContext);			//デバイスコンテキスト
	SAFE_RELEASE(pDevice);			//デバイス
}