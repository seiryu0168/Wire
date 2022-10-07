#pragma once
#include"Direct3D.h"
#include"Texture.h"
#include"Transform.h"
#include"SAFE_DELETE_RELEASE.h"

class Sprite
{
struct VERTEX
{
	XMVECTOR position;
	XMVECTOR uv;
};
//コンスタントバッファー
struct CONSTANT_BUFFER
{
	XMMATRIX	matPosition;
};
protected:
	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer* pIndexBuffer_;	//インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ
	Texture* pTexture_;

	DWORD vertexNum_;
	VERTEX* vertices_;
	DWORD indexNum_;
	int* index_;

	UINT imgWidth_;
	UINT imgHeight_;


public:
	Sprite();
	~Sprite();
	virtual HRESULT Initialize();
	HRESULT CreateVertexBuffer();
	HRESULT CreateIndexBuffer();
	HRESULT CreateConstantBuffer();
	HRESULT TextureSet(LPCWSTR fileName);

	virtual void InitVertex();
	virtual void InitIndex();

	void ToPipeLine(DirectX::XMMATRIX worldMatrix);
	void bufferSet();
	void Draw(Transform& transform);
	void Release();
};

