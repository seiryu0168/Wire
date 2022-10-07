#pragma once
#include <d3d11.h>
#include<assert.h>
#include <DirectXMath.h>
#include<vector>
#include"SAFE_DELETE_RELEASE.h"
using namespace DirectX;

//リンカ
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

enum SHADER_TYPE
{
	SHADER_2D = 0,
	SHADER_3D,
	SHADER_EFF,
	SHADER_MAX,
};
enum TARGET
{
	TARGET_BACK_BUFFER=0,
	TARGET_RENDER_TARGET_1,
	TARGET_RENDER_TARGET_2,
	TARGET_RENDER_TARGET_3,
	TARGET_RENDER_TARGET_MAX
};

enum RENDER_TYPE
{
	BACK_BUFFER=0,
	RENDER_TARGET,
	RENDER_MAX
};

namespace Direct3D
{
	extern ID3D11Device* pDevice;				//デバイス
	extern ID3D11DeviceContext* pContext;		//デバイスコンテキスト
	/*std::vector<RenderTarget*> renderTargetList_;
	int targetList[] =
	{
		BACK_BUFFER,
		RENDER_TARGET,
		RENDER_TARGET,
		RENDER_TARGET,
	};*/

	//初期化
	HRESULT Initialize(int winW, int winH, HWND hWnd);

	//シェーダー準備
    HRESULT InitShader();
	HRESULT InitShader2D();
	HRESULT InitShader3D();
	HRESULT InitShaderEFF();

	ID3D11BlendState* GetBlendState();
	//void SetRenderTargetView(int renderTarget);
	void SetShader(SHADER_TYPE type);
	/*ID3D11ShaderResourceView** GetSRV(int srvNum);
	IDXGISwapChain* GetSwapChain();
	ID3D11DepthStencilView* GetDepthStencilView();*/

	//描画開始
	void BeginDraw();


	//描画終了
	void EndDraw();

	//解放
	void Release();

};

//class RenderTarget
//{
//private:
//	ID3D11Texture2D* pLayerBuffer;
//	ID3D11RenderTargetView* pRenderTargetView;
//	ID3D11ShaderResourceView* pShaderResourceView;
//public:
//	RenderTarget();
//	~RenderTarget();
//
//	HRESULT Init(int renderType,int winW,int winH);
//	void SetShaderResource();
//	void SetRenderTarget();
//	void ClearRenderTarget();
//	ID3D11ShaderResourceView** GetSRV();
//	void Release();
//};