#include <d3dcompiler.h>
#include "Direct3D.h"
#include<array>

//変数
namespace Direct3D
{
	ID3D11Device* pDevice;									//デバイス
	ID3D11DeviceContext* pContext;							//デバイスコンテキスト
	IDXGISwapChain* pSwapChain;								//スワップチェイン
	ID3D11RenderTargetView* pRenderTargetView;				//レンダーターゲットビュー
	ID3D11BlendState* pBlendState[BLEND_MAX];				//ブレンドステート
	ID3D11Texture2D* pDepthStencil;							//深度ステンシル
	ID3D11DepthStencilView* pDepthStencilView;				//深度ステンシルビュー
	ID3D11DepthStencilState* pDepthStencilState[BLEND_MAX];	//デプスステンシルステート
	//ID3D11ShaderResourceView* pSRV[2];					//シェーダーリソースビュー
	//ID3D11Texture2D* pRenderTarget;


	struct SHADER_BUNDLE
	{
		ID3D11VertexShader* pVertexShader = nullptr;	//頂点シェーダー
		ID3D11PixelShader* pPixelShader = nullptr;		//ピクセルシェーダー
		ID3D11InputLayout* pVertexLayout = nullptr;	//頂点インプットレイアウト
		ID3D11RasterizerState* pRasterizerState = nullptr;	//ラスタライザー
	};
	SHADER_BUNDLE shaderBundle[SHADER_MAX];

	int screenWidth;
	int screenHeight;
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
		0,								// 何らかのフラグを指定する。（デバッグ時はD3D11_CREATE_DEVICE_DEBUG？）
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

	/*D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceView;
	ShaderResourceView.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	ShaderResourceView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceView.Texture2D.MipLevels = 1;
	Direct3D::pDevice->CreateShaderResourceView(pBackBuffer, &ShaderResourceView, &pSRV[0]);
	合成用のレンダーターゲット、レンダーターゲットビュー、シェーダーリソースビュー作成
	D3D11_TEXTURE2D_DESC rtDesc;
	ZeroMemory(&rtDesc, sizeof(rtDesc));
	rtDesc.Width = winW;
	rtDesc.Height = winH;
	rtDesc.MipLevels = 1;
	rtDesc.ArraySize = 1;
	rtDesc.Format = DXGI_FORMAT_D32_FLOAT;
	rtDesc.SampleDesc.Count = 1;
	rtDesc.SampleDesc.Quality = 0;
	rtDesc.Usage = D3D11_USAGE_DEFAULT;
	rtDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	rtDesc.CPUAccessFlags = 0;
	rtDesc.MiscFlags = 0;
	Direct3D::pDevice->CreateTexture2D(&rtDesc, NULL, &pRenderTarget);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"レンダーターゲットの作成に失敗", L"エラー", MB_OK);
		return hr;
	}
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"レンダーターゲットビューの作成に失敗", L"エラー", MB_OK);
		return hr;
	}
	D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
	srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv.Texture2D.MipLevels = 1;
	Direct3D::pDevice->CreateShaderResourceView(pRenderTarget, &srv, &pSRV[1]);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"シェーダーリソースビューの作成に失敗", L"エラー", MB_OK);
		return hr;
	}
	RenderTarget* pRenderTarget = new RenderTarget[4];

	for (int i = 0; i < RENDER_TARGET_MAX; i++)
	{
		pRenderTarget[i].Init(targetList[i], winW, winH);
		renderTargetList_.push_back(&pRenderTarget[i]);
	}*/

	
	//一時的にバックバッファを取得しただけなので解放
	SAFE_RELEASE(pBackBuffer);

	///////////////////////////ビューポート（描画範囲）設定///////////////////////////////
	//レンダリング結果を表示する範囲
	D3D11_VIEWPORT vp;
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

	pDevice->CreateDepthStencilState(&depthstencildesc, &pDepthStencilState[BLEND_DEFAULT]);
	pContext->OMSetDepthStencilState(pDepthStencilState[BLEND_DEFAULT], 0);

	depthstencildesc.StencilEnable = false;
	pDevice->CreateDepthStencilState(&depthstencildesc, &pDepthStencilState[BLEND_ADD]);

	//ブレンドステートの作成
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

	HRESULT hr;
	hr=pDevice->CreateBlendState(&desc, &pBlendState[BLEND_DEFAULT]);
	if(FAILED(hr))
	{
		MessageBox(nullptr, L"ブレンドステートの作成に失敗", L"エラー", MB_OK);
		return hr;
	}




	//データを画面に描画するための一通りの設定（パイプライン）
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);       //データの入力種類を指定
	pContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);        // 描画先を設定
	pContext->RSSetViewports(1, &vp);

	//シェーダー準備
	 hr=InitShader();
	 if (FAILED(hr))
	 {
		 return hr;
	 }

	 return S_OK;
}

//シェーダー準備
HRESULT Direct3D::InitShader()
{
	if (FAILED(InitShader2D()))
	{
		return E_FAIL;
	}
	if (FAILED(InitShader3D()))
	{
		return E_FAIL;
	}
	if(FAILED(InitShaderEFF()))
	{
		return E_FAIL;

	}
	return S_OK;

}

HRESULT Direct3D::InitShader2D()
{
	HRESULT hr;
	// 頂点シェーダの作成（コンパイル）
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"Simple2D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shaderBundle[SHADER_2D].pVertexShader);
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
	hr = pDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shaderBundle[SHADER_2D].pVertexLayout);
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
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &shaderBundle[SHADER_2D].pPixelShader);

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
	hr = pDevice->CreateRasterizerState(&rdc, &shaderBundle[SHADER_2D].pRasterizerState);
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
	D3DCompileFromFile(L"Simple3D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shaderBundle[SHADER_3D].pVertexShader);
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
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//位置
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMVECTOR),  D3D11_INPUT_PER_VERTEX_DATA, 0 },  //UV座標
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 2, D3D11_INPUT_PER_VERTEX_DATA, 0 },  //法線
	};

	hr = pDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shaderBundle[SHADER_3D].pVertexLayout);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pCompileVS);
		MessageBox(nullptr, L"頂点インプットレイアウトの設定に失敗", L"エラー", MB_OK);
		return hr;
	}
	SAFE_RELEASE(pCompileVS);

	// ピクセルシェーダの作成（コンパイル）
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"Simple3D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	assert(pCompilePS != nullptr);
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &shaderBundle[SHADER_3D].pPixelShader);
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
	hr = pDevice->CreateRasterizerState(&rdc, &shaderBundle[SHADER_3D].pRasterizerState);
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
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shaderBundle[SHADER_EFF].pVertexShader);
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
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &shaderBundle[SHADER_EFF].pPixelShader);
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
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	  0, sizeof(XMVECTOR),  D3D11_INPUT_PER_VERTEX_DATA, 0 },  //UV座標
	};

	hr = pDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shaderBundle[SHADER_EFF].pVertexLayout);
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
	hr = pDevice->CreateRasterizerState(&rdc, &shaderBundle[SHADER_EFF].pRasterizerState);
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
	pContext->VSSetShader(shaderBundle[type].pVertexShader, NULL, 0);	//頂点シェーダー
	pContext->PSSetShader(shaderBundle[type].pPixelShader, NULL, 0);	//ピクセルシェーダー
	pContext->IASetInputLayout(shaderBundle[type].pVertexLayout);	//頂点インプットレイアウト
	pContext->RSSetState(shaderBundle[type].pRasterizerState);		//ラスタライザー
}

void Direct3D::SetBlendMode(BLEND_MODE mode)
{
	float factor[4] = { D3D11_BLEND_ZERO,D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
	Direct3D::pContext->OMSetBlendState(Direct3D::GetBlendState(), factor, 0xffffffff);			//ブレンドステート
}
//描画開始
void Direct3D::BeginDraw()
{
	//画面をクリア
		//背景色
		float clearColor[4] = { 0.1,0.5,0.5,1.0 };
		//レンダーターゲットビューをクリア
		pContext->ClearRenderTargetView(pRenderTargetView, clearColor);

	//深度バッファクリア
	pContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

//描画終了
void Direct3D::EndDraw()
{
	//スワップ（バックバッファを表に表示する）
	pSwapChain->Present(0, 0);
}

//解放処理
void Direct3D::Release()
{
	//解放処理
	for (int releaseShader = 0; releaseShader < SHADER_MAX; releaseShader++)
	{
		SAFE_RELEASE(shaderBundle[releaseShader].pRasterizerState);
		SAFE_RELEASE(shaderBundle[releaseShader].pVertexLayout);
		SAFE_RELEASE(shaderBundle[releaseShader].pPixelShader);
		SAFE_RELEASE(shaderBundle[releaseShader].pVertexShader);
	}
	SAFE_RELEASE(pDepthStencilView);				//深度ステンシルビュー
	SAFE_RELEASE(pDepthStencil);
	for (int i = 0; i < BLEND_MAX; i++)
	{
		SAFE_DELETE(pDepthStencilState[i]);
		SAFE_RELEASE(pBlendState[i]);				//深度ステンシル
	}
	SAFE_RELEASE(pRenderTargetView);
	SAFE_RELEASE(pSwapChain);		//スワップチェイン
	SAFE_RELEASE(pContext);			//デバイスコンテキスト
	SAFE_RELEASE(pDevice);			//デバイス
}


//ID3D11BlendState* Direct3D::GetBlendState()
//{
//	return pBlendState[];
//}

//void Direct3D::SetRenderTargetView(int renderTarget)
//{
//	pContext->OMSetRenderTargets(1, &pRenderTargetView[renderTarget], pDepthStencilView);
//}
//RenderTarget::RenderTarget()
//	:pLayerBuffer(nullptr),pRenderTargetView(nullptr),pShaderResourceView(nullptr)
//{
//
//}
//RenderTarget::~RenderTarget()
//{
//	SAFE_DELETE(pShaderResourceView);
//	SAFE_DELETE(pRenderTargetView);
//	SAFE_DELETE(pLayerBuffer);
//}
//
//HRESULT RenderTarget::Init(int renderType,int winW,int winH)
//{
//	HRESULT hr;
//	switch(renderType)
//	{
//	case BACK_BUFFER:
//	
//		Direct3D::pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pLayerBuffer);
//		Direct3D::pDevice->CreateRenderTargetView(pLayerBuffer, NULL, &pRenderTargetView);
//		break;
//	
//	case RENDER_TARGET:
//		D3D11_TEXTURE2D_DESC rtDesc;
//		ZeroMemory(&rtDesc, sizeof(rtDesc));
//		rtDesc.Width = winW;
//		rtDesc.Height = winH;
//		rtDesc.MipLevels = 1;
//		rtDesc.ArraySize = 1;
//		rtDesc.Format = DXGI_FORMAT_D32_FLOAT;
//		rtDesc.SampleDesc.Count = 1;
//		rtDesc.SampleDesc.Quality = 0;
//		rtDesc.Usage = D3D11_USAGE_DEFAULT;
//		rtDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
//		rtDesc.CPUAccessFlags = 0;
//		rtDesc.MiscFlags = 0;
//		hr = Direct3D::pDevice->CreateTexture2D(&rtDesc, NULL, &pLayerBuffer);
//		if (FAILED(hr))
//		{
//			MessageBox(nullptr, L"レンダーターゲットの作成に失敗",L"エラー", MB_OK);
//			return hr;
//		}
//
//		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
//		ZeroMemory(&rtvDesc, sizeof(rtvDesc));
//		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
//
//		hr = Direct3D::pDevice->CreateRenderTargetView(pLayerBuffer, &rtvDesc, &pRenderTargetView);
//		if (FAILED(hr))
//		{
//			MessageBox(nullptr, L"レンダーターゲットビューの作成に失敗", L"エラー", MB_OK);
//			return hr;
//		}
//		D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
//		srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//		srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
//		srv.Texture2D.MipLevels = 1;
//		hr = Direct3D::pDevice->CreateShaderResourceView(pLayerBuffer, &srv, &pShaderResourceView);
//		if (FAILED(hr))
//		{
//			MessageBox(nullptr, L"シェーダーリソースビューの作成に失敗", L"エラー", MB_OK);
//			return hr;
//		}
//		return S_OK;
//	}
//
//	
//}
//void RenderTarget::SetShaderResource()
//{
//}
//void RenderTarget::SetRenderTarget()
//{
//	Direct3D::pContext->OMSetRenderTargets(1, &pRenderTargetView, Direct3D::pDepthStencilView);
//}
//void RenderTarget::ClearRenderTarget()
//{
//	//背景色
//	float clearColor[4]={ 0.5,0.5,0.5,1.0 };
//	//レンダーターゲットビューをクリア
//	Direct3D::pContext->ClearRenderTargetView(pRenderTargetView, clearColor);
//}
//ID3D11ShaderResourceView** RenderTarget::GetSRV()
//{
//	return &pShaderResourceView;
//}
//
//void RenderTarget::Release()
//{
//	SAFE_DELETE(pShaderResourceView);
//	SAFE_DELETE(pRenderTargetView);
//	SAFE_DELETE(pLayerBuffer);
//}