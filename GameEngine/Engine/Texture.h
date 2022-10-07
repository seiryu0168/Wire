#pragma once
#include <d3d11.h>
#include "string"

class Texture
{

	ID3D11SamplerState* pSampler_;		//サンプラー
	ID3D11ShaderResourceView* pSRV_;	//シェーダーリソースビュー

	UINT imgWidth_;
	UINT imgHeight_;
public:
	Texture();
	~Texture();
	HRESULT Load(LPCWSTR fileName);
	ID3D11SamplerState* GetSampler(){ return pSampler_; }
	ID3D11ShaderResourceView* GetSRV() { return pSRV_; }

	UINT GetWidth();
	UINT GetHeight();
	void Release();
};