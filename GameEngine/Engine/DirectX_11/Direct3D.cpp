#include <d3dcompiler.h>
#include"../../EngineTime.h"
#include "Direct3D.h"
#include<array>

//�ϐ�
namespace Direct3D
{
	ID3D11Device*			 pDevice;										//�f�o�C�X
	ID3D11DeviceContext*	 pContext;										//�f�o�C�X�R���e�L�X�g
	IDXGISwapChain*			 pSwapChain;									//�X���b�v�`�F�C��
	ID3D11RenderTargetView*	 pRenderTargetView;								//�����_�[�^�[�Q�b�g�r���[
	ID3D11Texture2D*		 pDepthStencil;									//�[�x�X�e���V��
	ID3D11DepthStencilView*	 pDepthStencilView;								//�[�x�X�e���V���r���[
	ID3D11BlendState*		 pBlendState[(int)BLEND_MODE::BLEND_MAX];		//�u�����h�X�e�[�g
	ID3D11DepthStencilState* pDepthStencilState[(int)BLEND_MODE::BLEND_MAX];//�f�v�X�X�e���V���X�e�[�g


	//�[�x�e�N�X�`���p
	ID3D11Texture2D*		  pDepthTexture;			//�`���
	ID3D11RenderTargetView*	  pDepthRenderTargetView;	//�����_�[�^�[�Q�b�g
	ID3D11DepthStencilView*	  pDepthDepthStencilView;	//�X�e���V���o�b�t�@
	ID3D11Texture2D*		  pDepthDepthStencil;		//�[�x�o�b�t�@�p�e�N�X�`��
	ID3D11ShaderResourceView* pDepthSRV;		//�[�x�e�N�X�`�����V�F�[�_�[�ɓn�����
	ID3D11VertexShader*	      pDepthVertexShader;		//�e�p���_�V�F�[�_�[
	ID3D11PixelShader*		  pDepthPixelShader;		//�e�p�s�N�Z���V�F�[�_�[
	D3D11_VIEWPORT depthVp;

	struct SHADER_BUNDLE
	{
		ID3D11VertexShader* pVertexShader = nullptr;	//���_�V�F�[�_�[
		ID3D11PixelShader* pPixelShader = nullptr;		//�s�N�Z���V�F�[�_�[
		ID3D11InputLayout* pVertexLayout = nullptr;		//���_�C���v�b�g���C�A�E�g
		ID3D11RasterizerState* pRasterizerState = nullptr;	//���X�^���C�U�[
	};
	SHADER_BUNDLE shaderBundle[(int)SHADER_TYPE::SHADER_MAX];
	D3D11_VIEWPORT vp;
	int screenWidth;
	int screenHeight;
	XMMATRIX clipToUV;
	XMMATRIX lightView;
	ID3D11SamplerState* pDepthSampler;
	bool useShadow = true;
	bool shadowRender;

}

//������
HRESULT Direct3D::Initialize(int winW, int winH, HWND hWnd)
{

	screenWidth = winW;
	screenHeight = winH;
	///////////////////////////���낢�돀�����邽�߂̐ݒ�///////////////////////////////
	//���낢��Ȑݒ荀�ڂ��܂Ƃ߂��\����
	DXGI_SWAP_CHAIN_DESC scDesc;

	//�Ƃ肠�����S��0
	ZeroMemory(&scDesc, sizeof(scDesc));

	//�`���̃t�H�[�}�b�g
	scDesc.BufferDesc.Width = winW;		//��ʕ�
	scDesc.BufferDesc.Height = winH;	//��ʍ���
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// ���F�g���邩

	//FPS�i1/60�b��1��j
	scDesc.BufferDesc.RefreshRate.Numerator = 60;
	scDesc.BufferDesc.RefreshRate.Denominator = 1;

	//���̑�
	scDesc.Windowed = TRUE;			//�E�B���h�E���[�h���t���X�N���[����
	scDesc.OutputWindow = hWnd;		//�E�B���h�E�n���h��
	scDesc.BufferCount = 1;			//�o�b�N�o�b�t�@�̖���
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//�o�b�N�o�b�t�@�̎g��������ʂɕ`�悷�邽�߂�
	scDesc.SampleDesc.Count = 1;		//MSAA�i�A���`�G�C���A�X�j�̐ݒ�
	scDesc.SampleDesc.Quality = 0;		//�@�V

	////////////////��L�ݒ�����ƂɃf�o�C�X�A�R���e�L�X�g�A�X���b�v�`�F�C�����쐬////////////////////////
	D3D_FEATURE_LEVEL level;
	D3D11CreateDeviceAndSwapChain(
		nullptr,						// �ǂ̃r�f�I�A�_�v�^���g�p���邩�H����Ȃ��nullptr��
		D3D_DRIVER_TYPE_HARDWARE,		// �h���C�o�̃^�C�v��n���B�ӂ���HARDWARE
		nullptr,						// ��L��D3D_DRIVER_TYPE_SOFTWARE�ɐݒ肵�Ȃ�������nullptr
		D3D10_CREATE_DEVICE_BGRA_SUPPORT,//D3D11_CREATE_DEVICE_DISABLE_GPU_TIMEOUT,								// ���炩�̃t���O���w�肷��B�i�f�o�b�O����D3D11_CREATE_DEVICE_DEBUG�H�j
		nullptr,						// �f�o�C�X�A�R���e�L�X�g�̃��x����ݒ�Bnullptr�ɂ��Ƃ���OK
		0,								// ��̈����Ń��x�������w�肵����
		D3D11_SDK_VERSION,				// SDK�̃o�[�W�����B�K�����̒l
		&scDesc,						// ��ł��낢��ݒ肵���\����
		&pSwapChain,					// ������������SwapChain�̃A�h���X���Ԃ��Ă���
		&pDevice,						// ������������Device�A�h���X���Ԃ��Ă���
		&level,							// ������������Device�AContext�̃��x�����Ԃ��Ă���
		&pContext);						// ������������Context�̃A�h���X���Ԃ��Ă���

	///////////////////////////�����_�[�^�[�Q�b�g�r���[�쐬///////////////////////////////
	//�X���b�v�`�F�[������o�b�N�o�b�t�@���擾�i�o�b�N�o�b�t�@ �� �����_�[�^�[�Q�b�g�j

	//�����_�[�^�[�Q�b�g�r���[���쐬
	ID3D11Texture2D* pBackBuffer;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);

	//�ꎞ�I�Ƀo�b�N�o�b�t�@���擾���������Ȃ̂ŉ��
	SAFE_RELEASE(pBackBuffer);

	///////////////////////////�r���[�|�[�g�i�`��͈́j�ݒ�///////////////////////////////
	//�����_�����O���ʂ�\������͈�
	vp;
	vp.Width = (float)winW;	//��
	vp.Height = (float)winH;//����
	vp.MinDepth = 0.0f;	//��O
	vp.MaxDepth = 1.0f;	//��
	vp.TopLeftX = 0;	//��
	vp.TopLeftY = 0;	//��


	//�[�x�X�e���V���r���[�̍쐬
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


	//�[�x�X�e���V���X�e�[�g�̍쐬
	D3D11_DEPTH_STENCIL_DESC depthstencildesc = {};
	depthstencildesc.DepthEnable = true;
	depthstencildesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthstencildesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthstencildesc.StencilEnable = true;

	pDevice->CreateDepthStencilState(&depthstencildesc, &pDepthStencilState[(int)BLEND_MODE::BLEND_DEFAULT]);
	pContext->OMSetDepthStencilState(pDepthStencilState[(int)BLEND_MODE::BLEND_DEFAULT], 0);

	depthstencildesc.StencilEnable = false;
	pDevice->CreateDepthStencilState(&depthstencildesc, &pDepthStencilState[(int)BLEND_MODE::BLEND_ADD]);

	//�u�����h�X�e�[�g�̍쐬

	//�f�t�H���g
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
		MessageBox(nullptr, L"�u�����h�X�e�[�g�̍쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}
	pContext->OMSetBlendState(pBlendState[(int)BLEND_MODE::BLEND_DEFAULT], factor, 0xffffffff);
	//���Z����
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	pDevice->CreateBlendState(&desc, &pBlendState[(int)BLEND_MODE::BLEND_ADD]);



	//�f�[�^����ʂɕ`�悷�邽�߂̈�ʂ�̐ݒ�i�p�C�v���C���j
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);       //�f�[�^�̓��͎�ނ��w��
	pContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);        // �`����ݒ�
	pContext->RSSetViewports(1, &vp);


	hr=InitDepthTexture();
	if (FAILED(hr))
		return hr;

	//�V�F�[�_�[����
	 hr=InitShader();
	 if (FAILED(hr))
	 {
		 return hr;
	 }

	

	 return S_OK;
}

HRESULT Direct3D::InitDepthTexture()
{
	HRESULT hr;

	//�[�x�e�N�X�`���̕`���쐬
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
		MessageBox(nullptr, L"�[�x�e�N�X�`���쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}

	//�����_�[�^�[�Q�b�g�r���[�쐬
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	hr = pDevice->CreateRenderTargetView(pDepthTexture, &rtvDesc, &pDepthRenderTargetView);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"�[�x�e�N�X�`���p�����_�[�^�[�Q�b�g�r���[�쐬�Ɏ��s", L"�G���[", MB_OK);
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
		MessageBox(nullptr, L"�[�x�e�N�X�`���p�f�v�X�X�e���V���쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}

	hr = pDevice->CreateDepthStencilView(pDepthDepthStencil, NULL, &pDepthDepthStencilView);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"�[�x�e�N�X�`���p�f�v�X�X�e���V���r���[�쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}

	//�[�x�e�N�X�`���p�V�F�[�_�[���\�[�X�r���[�쐬
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	hr = pDevice->CreateShaderResourceView(pDepthTexture, &srvDesc, &pDepthSRV);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"�[�x�e�N�X�`���p�V�F�[�_�[���\�[�X�r���[�쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}

	XMFLOAT4X4 clip;
	ZeroMemory(&clip, sizeof(XMFLOAT4X4));
	clip._11 = 0.5;
	clip._22 = -0.5;
	clip._33 = 1;
	clip._41 = 0.5;
	clip._42 = 0.5;
	clip._44 = 1;
	clipToUV = XMLoadFloat4x4(&clip);

	depthVp.Width = (float)screenWidth;	//��
	depthVp.Height = (float)screenHeight;//����
	depthVp.MinDepth = 0.0f;	//��O
	depthVp.MaxDepth = 1.0f;	//��
	depthVp.TopLeftX = 0;	//��
	depthVp.TopLeftY = 0;	//��

	//�[�x�e�N�X�`���p�T���v���[�쐬
	D3D11_SAMPLER_DESC  depthSmplDesc;
	ZeroMemory(&depthSmplDesc, sizeof(D3D11_SAMPLER_DESC));
	depthSmplDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	depthSmplDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	depthSmplDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	depthSmplDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	lightView = XMMatrixLookAtLH(XMVectorSet(0, 600, 0, 0),
		XMVectorSet(0, 0,-1, 0),
		XMVectorSet(0, 1, 0, 0));

	hr = Direct3D::pDevice->CreateSamplerState(&depthSmplDesc, &pDepthSampler);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Texture:�T���v���[�쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		return hr;
	}
	return TRUE;
}

//�V�F�[�_�[����
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
	// ���_�V�F�[�_�̍쐬�i�R���p�C���j
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"Simple2D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL,
									 &shaderBundle[(int)SHADER_TYPE::SHADER_2D].pVertexShader);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pCompileVS);
		MessageBox(nullptr, L"���_�V�F�[�_�̍쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}

	//���_�C���v�b�g���C�A�E�g   ���_�V�F�[�_�\�ɓn�����
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//�ʒu
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMVECTOR),  D3D11_INPUT_PER_VERTEX_DATA, 0 },  //UV���W
	};
	hr = pDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), pCompileVS->GetBufferPointer(),
									pCompileVS->GetBufferSize(), &shaderBundle[(int)SHADER_TYPE::SHADER_2D].pVertexLayout);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pCompileVS);
		MessageBox(nullptr, L"���_�C���v�b�g���C�A�E�g�̐ݒ�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}
	SAFE_RELEASE(pCompileVS);

	// �s�N�Z���V�F�[�_�̍쐬�i�R���p�C���j
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"Simple2D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	assert(pCompilePS != nullptr);
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL,
									&shaderBundle[(int)SHADER_TYPE::SHADER_2D].pPixelShader);

	if (FAILED(hr))
	{
		SAFE_RELEASE(pCompilePS);
		MessageBox(nullptr, L"�s�N�Z���V�F�[�_�̍쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}
	SAFE_RELEASE(pCompilePS);

	//���X�^���C�U�쐬
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE;
	hr = pDevice->CreateRasterizerState(&rdc, &shaderBundle[(int)SHADER_TYPE::SHADER_2D].pRasterizerState);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pCompilePS);
		MessageBox(nullptr, L"�s�N�Z���V�F�[�_�̍쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}

	return S_OK;
}

HRESULT Direct3D::InitShader3D()
{
	HRESULT hr;
	// ���_�V�F�[�_�̍쐬�i�R���p�C���j
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"newSimple3D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL,
									 &shaderBundle[(int)SHADER_TYPE::SHADER_3D].pVertexShader);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pCompileVS);
		MessageBox(nullptr, L"���_�V�F�[�_�̍쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}

	//////////////////////////////////////////////////���_�C���v�b�g���C�A�E�g///////////////////////////////////////////////
	//HLSL(�V�F�[�_�[�̃\�[�X�R�[�h)�ɑ�����̎�ނƂ��̐ݒ���s��
	//1.�Z�}���e�B�b�N�̖��O
	//2.�Z�}���e�B�b�N�C���f�b�N�X(�����Z�}���e�B�b�N�����v�f����������Ƃ��Ɏg��)
	//3.�v�f�f�[�^�̃f�[�^�f�[�^�^
	//4.���̓A�Z���u�������ʂ��鐮���l(0�`15)
	//5.���_�̐擪�A�h���X����̃I�t�Z�b�g(�o�C�g�P��)
	//6.���̓X���b�g�̓��̓f�[�^�N���X�̎���
	//7.�o�b�t�@����1�v�f�i�ޑO�ɁA�����C���X�^���X���Ƃ̃f�[�^���g�p����
	//�`�悷��C���X�^���X�̐����_�P�ʂ̃f�[�^���܂ޗv�f�̏ꍇ�A0�ɂ���E�E�E�炵��
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,				0,	   D3D11_INPUT_PER_VERTEX_DATA, 0 },  //�ʒu
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, sizeof(XMVECTOR),	   D3D11_INPUT_PER_VERTEX_DATA, 0 },  //UV���W
		{ "NORMAL",	  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 2, D3D11_INPUT_PER_VERTEX_DATA, 0 },  //�@��
		{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },  //�ڐ�
	};

	hr = pDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), pCompileVS->GetBufferPointer(), 
									pCompileVS->GetBufferSize(), &shaderBundle[(int)SHADER_TYPE::SHADER_3D].pVertexLayout);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pCompileVS);
		MessageBox(nullptr, L"���_�C���v�b�g���C�A�E�g�̐ݒ�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}
	SAFE_RELEASE(pCompileVS);

	// �s�N�Z���V�F�[�_�̍쐬�i�R���p�C���j
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"newSimple3D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	assert(pCompilePS != nullptr);
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL,
									&shaderBundle[(int)SHADER_TYPE::SHADER_3D].pPixelShader);
	SAFE_RELEASE(pCompilePS);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"�s�N�Z���V�F�[�_�̍쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}

	//���X�^���C�U�쐬
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE;
	hr = pDevice->CreateRasterizerState(&rdc, &shaderBundle[(int)SHADER_TYPE::SHADER_3D].pRasterizerState);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���X�^���C�U�̍쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}

	return S_OK;
}

HRESULT Direct3D::InitShaderEFF()
{
	HRESULT hr;
	// ���_�V�F�[�_�̍쐬�i�R���p�C���j
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"BillBoard.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL,
									 &shaderBundle[(int)SHADER_TYPE::SHADER_EFF].pVertexShader);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pCompileVS);
		MessageBox(nullptr, L"���_�V�F�[�_�̍쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}

	// �s�N�Z���V�F�[�_�̍쐬�i�R���p�C���j
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"BillBoard.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	assert(pCompilePS != nullptr);
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL,
									&shaderBundle[(int)SHADER_TYPE::SHADER_EFF].pPixelShader);
	SAFE_RELEASE(pCompilePS);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"�s�N�Z���V�F�[�_�̍쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}

	//////////////////////////////////////////////////���_�C���v�b�g���C�A�E�g///////////////////////////////////////////////
	//HLSL(�V�F�[�_�[�̎�)�ɑ�����̎�ނƂ��̐ݒ���s��
	//1.�Z�}���e�B�b�N�̖��O
	//2.�Z�}���e�B�b�N�C���f�b�N�X(�����Z�}���e�B�b�N�����v�f����������Ƃ��Ɏg��)
	//3.�v�f�f�[�^�̃f�[�^�f�[�^�^
	//4.���̓A�Z���u�������ʂ��鐮���l(0�`15)
	//5.���_�̐擪�A�h���X����̃I�t�Z�b�g(�o�C�g�P��)
	//6.���̓X���b�g�̓��̓f�[�^�N���X�̎���
	//7.�o�b�t�@����1�v�f�i�ޑO�ɁA�����C���X�^���X���Ƃ̃f�[�^���g�p����
	//�`�悷��C���X�^���X�̐����_�P�ʂ̃f�[�^���܂ޗv�f�̏ꍇ�A0�ɂ���E�E�E�炵��
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,					D3D11_INPUT_PER_VERTEX_DATA, 0 },  //�ʒu
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	  0, sizeof(XMFLOAT3),  D3D11_INPUT_PER_VERTEX_DATA, 0 },  //UV���W
	};

	hr = pDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), pCompileVS->GetBufferPointer(),
									pCompileVS->GetBufferSize(), &shaderBundle[(int)SHADER_TYPE::SHADER_EFF].pVertexLayout);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pCompileVS);
		MessageBox(nullptr, L"���_�C���v�b�g���C�A�E�g�̐ݒ�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}
	SAFE_RELEASE(pCompileVS);


	//���X�^���C�U�쐬
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_NONE;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise =TRUE;
	hr = pDevice->CreateRasterizerState(&rdc, &shaderBundle[(int)SHADER_TYPE::SHADER_EFF].pRasterizerState);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���X�^���C�U�̍쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}

	return S_OK;
}

HRESULT Direct3D::InitShaderOutLine()
{
	HRESULT hr;
	// ���_�V�F�[�_�̍쐬�i�R���p�C���j
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"OutLine.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, 
									 &shaderBundle[(int)SHADER_TYPE::SHADER_OUTLINE].pVertexShader);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pCompileVS);
		MessageBox(nullptr, L"���_�V�F�[�_�̍쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}

	//////////////////////////////////////////////////���_�C���v�b�g���C�A�E�g///////////////////////////////////////////////
	//HLSL(�V�F�[�_�[�̎�)�ɑ�����̎�ނƂ��̐ݒ���s��
	//1.�Z�}���e�B�b�N�̖��O
	//2.�Z�}���e�B�b�N�C���f�b�N�X(�����Z�}���e�B�b�N�����v�f����������Ƃ��Ɏg��)
	//3.�v�f�f�[�^�̃f�[�^�f�[�^�^
	//4.���̓A�Z���u�������ʂ��鐮���l(0�`15)
	//5.���_�̐擪�A�h���X����̃I�t�Z�b�g(�o�C�g�P��)
	//6.���̓X���b�g�̓��̓f�[�^�N���X�̎���
	//7.�o�b�t�@����1�v�f�i�ޑO�ɁA�����C���X�^���X���Ƃ̃f�[�^���g�p����
	//�`�悷��C���X�^���X�̐����_�P�ʂ̃f�[�^���܂ޗv�f�̏ꍇ�A0�ɂ���E�E�E�炵��
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,				0,	   D3D11_INPUT_PER_VERTEX_DATA, 0 },  //�ʒu
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, sizeof(XMVECTOR),	   D3D11_INPUT_PER_VERTEX_DATA, 0 },  //UV���W
		{ "NORMAL",	  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 2, D3D11_INPUT_PER_VERTEX_DATA, 0 },  //�@��
		{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },  //�ڐ�
	};

	hr = pDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), pCompileVS->GetBufferPointer(),
									pCompileVS->GetBufferSize(), &shaderBundle[(int)SHADER_TYPE::SHADER_OUTLINE].pVertexLayout);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pCompileVS);
		MessageBox(nullptr, L"���_�C���v�b�g���C�A�E�g�̐ݒ�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}
	SAFE_RELEASE(pCompileVS);

	// �s�N�Z���V�F�[�_�̍쐬�i�R���p�C���j
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"OutLine.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	assert(pCompilePS != nullptr);
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL,
									&shaderBundle[(int)SHADER_TYPE::SHADER_OUTLINE].pPixelShader);
	SAFE_RELEASE(pCompilePS);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"�s�N�Z���V�F�[�_�̍쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}

	//���X�^���C�U�쐬
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_FRONT;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE;
	hr = pDevice->CreateRasterizerState(&rdc, &shaderBundle[(int)SHADER_TYPE::SHADER_OUTLINE].pRasterizerState);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���X�^���C�U�̍쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}

	return S_OK;
}

HRESULT Direct3D::InitShaderToon()
{
	HRESULT hr;
	// ���_�V�F�[�_�̍쐬�i�R���p�C���j
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"ToonShader.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL,
									 &shaderBundle[(int)SHADER_TYPE::SHADER_TOON].pVertexShader);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pCompileVS);
		MessageBox(nullptr, L"���_�V�F�[�_�̍쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}

	//////////////////////////////////////////////////���_�C���v�b�g���C�A�E�g///////////////////////////////////////////////
	//HLSL(�V�F�[�_�[�̎�)�ɑ�����̎�ނƂ��̐ݒ���s��
	//1.�Z�}���e�B�b�N�̖��O
	//2.�Z�}���e�B�b�N�C���f�b�N�X(�����Z�}���e�B�b�N�����v�f����������Ƃ��Ɏg��)
	//3.�v�f�f�[�^�̃f�[�^�f�[�^�^
	//4.���̓A�Z���u�������ʂ��鐮���l(0�`15)
	//5.���_�̐擪�A�h���X����̃I�t�Z�b�g(�o�C�g�P��)
	//6.���̓X���b�g�̓��̓f�[�^�N���X�̎���
	//7.�o�b�t�@����1�v�f�i�ޑO�ɁA�����C���X�^���X���Ƃ̃f�[�^���g�p����
	//�`�悷��C���X�^���X�̐����_�P�ʂ̃f�[�^���܂ޗv�f�̏ꍇ�A0�ɂ���E�E�E�炵��
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,				0,	   D3D11_INPUT_PER_VERTEX_DATA, 0 },  //�ʒu
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, sizeof(XMVECTOR),	   D3D11_INPUT_PER_VERTEX_DATA, 0 },  //UV���W
		{ "NORMAL",	  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 2, D3D11_INPUT_PER_VERTEX_DATA, 0 },  //�@��
		{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },  //�ڐ�
	};

	hr = pDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), 
									&shaderBundle[(int)SHADER_TYPE::SHADER_TOON].pVertexLayout);
	if (FAILED(hr))
	{
		SAFE_RELEASE(pCompileVS);
		MessageBox(nullptr, L"���_�C���v�b�g���C�A�E�g�̐ݒ�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}
	SAFE_RELEASE(pCompileVS);

	// �s�N�Z���V�F�[�_�̍쐬�i�R���p�C���j
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"ToonShader.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	assert(pCompilePS != nullptr);
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, 
									&shaderBundle[(int)SHADER_TYPE::SHADER_TOON].pPixelShader);
	SAFE_RELEASE(pCompilePS);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"�s�N�Z���V�F�[�_�̍쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}

	//���X�^���C�U�쐬
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE;
	hr = pDevice->CreateRasterizerState(&rdc, &shaderBundle[(int)SHADER_TYPE::SHADER_TOON].pRasterizerState);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���X�^���C�U�̍쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}

	return S_OK;
}

HRESULT Direct3D::InitShaderShadow()
{
	HRESULT hr;
	// ���_�V�F�[�_�̍쐬�i�R���p�C���j
	ID3DBlob* pCompileVS = nullptr;
	ID3DBlob* pVSError = nullptr;
	hr=D3DCompileFromFile(L"DepthShader.hlsl", nullptr, nullptr, "VS_Depth", "vs_5_0", NULL, 0, &pCompileVS, &pVSError);
	assert(pCompileVS != nullptr);
	if (FAILED(hr))
	{
		if (pVSError != nullptr)
		{
			MessageBox(nullptr, (LPCWSTR)pVSError->GetBufferPointer(), L"�G���[", MB_OK);
		}

		SAFE_RELEASE(pVSError);
		SAFE_RELEASE(pCompileVS);
		MessageBox(nullptr, L"���_�V�F�[�_�̍쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}
	SAFE_RELEASE(pVSError);
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL,
		&shaderBundle[(int)SHADER_TYPE::SHADER_DEPTH].pVertexShader);

	//////////////////////////////////////////////////���_�C���v�b�g���C�A�E�g///////////////////////////////////////////////
	//HLSL(�V�F�[�_�[�̃\�[�X�R�[�h)�ɑ�����̎�ނƂ��̐ݒ���s��
	//1.�Z�}���e�B�b�N�̖��O
	//2.�Z�}���e�B�b�N�C���f�b�N�X(�����Z�}���e�B�b�N�����v�f����������Ƃ��Ɏg��)
	//3.�v�f�f�[�^�̃f�[�^�f�[�^�^
	//4.���̓A�Z���u�������ʂ��鐮���l(0�`15)
	//5.���_�̐擪�A�h���X����̃I�t�Z�b�g(�o�C�g�P��)
	//6.���̓X���b�g�̓��̓f�[�^�N���X�̎���
	//7.�o�b�t�@����1�v�f�i�ޑO�ɁA�����C���X�^���X���Ƃ̃f�[�^���g�p����
	//�`�悷��C���X�^���X�̐����_�P�ʂ̃f�[�^���܂ޗv�f�̏ꍇ�A0�ɂ���E�E�E�炵��
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },  //�ʒu
	};

	hr = pDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), pCompileVS->GetBufferPointer(),
		pCompileVS->GetBufferSize(), &shaderBundle[(int)SHADER_TYPE::SHADER_DEPTH].pVertexLayout);
	if (FAILED(hr))
	{
		
		SAFE_RELEASE(pCompileVS);
		MessageBox(nullptr, L"���_�C���v�b�g���C�A�E�g�̐ݒ�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}
	SAFE_RELEASE(pCompileVS);

	// �s�N�Z���V�F�[�_�̍쐬�i�R���p�C���j
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
			MessageBox(nullptr, (LPCWSTR)pPSError->GetBufferPointer(), L"�G���[", MB_OK);
		}
		SAFE_RELEASE(pPSError);
		MessageBox(nullptr, L"�s�N�Z���V�F�[�_�̍쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}
	SAFE_RELEASE(pPSError);

	//���X�^���C�U�쐬
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE;
	hr = pDevice->CreateRasterizerState(&rdc, &shaderBundle[(int)SHADER_TYPE::SHADER_DEPTH].pRasterizerState);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���X�^���C�U�̍쐬�Ɏ��s", L"�G���[", MB_OK);
		return hr;
	}

	return S_OK;
}

void Direct3D::SetShader(SHADER_TYPE type)
{
	//���ꂼ����f�o�C�X�R���e�L�X�g�ɃZ�b�g  �����̏����g���ĕ`����s��
	pContext->VSSetShader(shaderBundle[(int)type].pVertexShader, NULL, 0);	//���_�V�F�[�_�[
	pContext->PSSetShader(shaderBundle[(int)type].pPixelShader, NULL, 0);	//�s�N�Z���V�F�[�_�[
	pContext->IASetInputLayout(shaderBundle[(int)type].pVertexLayout);	//���_�C���v�b�g���C�A�E�g
	pContext->RSSetState(shaderBundle[(int)type].pRasterizerState);		//���X�^���C�U�[
}

void Direct3D::SetBlendMode(BLEND_MODE mode)
{
	float factor[4] = { D3D11_BLEND_ZERO,D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
	Direct3D::pContext->OMSetBlendState(pBlendState[(int)mode], factor, 0xffffffff);			//�u�����h�X�e�[�g

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
	pContext->OMSetRenderTargets(1, &pDepthRenderTargetView, pDepthStencilView);
	pContext->ClearRenderTargetView(pDepthRenderTargetView, color);
	pContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	pContext->RSSetViewports(1,&depthVp);
	shadowRender = true;
}
void Direct3D::EndShadowDraw()
{
	shadowRender = false;
}
//�`��J�n
void Direct3D::BeginDraw()
{
	EngineTime::SetTime();
	//��ʂ��N���A
		//�w�i�F
		float clearColor[4] = { 0.1f,0.5f,0.5f,1.0f };
		pContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthDepthStencilView);
		//�����_�[�^�[�Q�b�g�r���[���N���A
		pContext->ClearRenderTargetView(pRenderTargetView, clearColor);

	//�[�x�o�b�t�@�N���A
	pContext->ClearDepthStencilView(pDepthDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	pContext->RSSetViewports(1, &vp);
}

//�`��I��
void Direct3D::EndDraw()
{
	EngineTime::SetTime();
	//�X���b�v�i�o�b�N�o�b�t�@��\�ɕ\������j
	HRESULT hr = pSwapChain->Present(0, 0);
	if (hr== DXGI_ERROR_DEVICE_REMOVED || hr== DXGI_ERROR_DEVICE_RESET)
	{
		
			MessageBox(nullptr, L"Direct3D : �X���b�v�`�F�C�����s", L"�G���[", MB_OK);
		
	}
}

//�������
void Direct3D::Release()
{
	//�������
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
		SAFE_RELEASE(pBlendState[i]);				//�[�x�X�e���V��
	}
	SAFE_RELEASE(pDepthSRV);
	SAFE_RELEASE(pDepthDepthStencil);
	SAFE_RELEASE(pDepthDepthStencilView);
	SAFE_RELEASE(pDepthRenderTargetView);
	SAFE_RELEASE(pDepthTexture);
	SAFE_RELEASE(pDepthStencilView);				//�[�x�X�e���V���r���[
	SAFE_RELEASE(pDepthStencil);
	SAFE_RELEASE(pRenderTargetView);
	SAFE_RELEASE(pSwapChain);		//�X���b�v�`�F�C��
	SAFE_RELEASE(pContext);			//�f�o�C�X�R���e�L�X�g
	SAFE_RELEASE(pDevice);			//�f�o�C�X
}