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

	DWORD vertexNum_;
	VERTEX* vertices_;
	DWORD indexNum_;
	int* index_;
	
	XMUINT2 imgSize_;
	int hPict_;
	HRESULT CreateVertexBuffer();
	HRESULT CreateIndexBuffer();
	HRESULT CreateConstantBuffer();
	HRESULT Load(std::string fileName);

	void ToPipeLine(DirectX::XMMATRIX worldMatrix);
	void bufferSet();
	void Release();

public:
	Sprite();
	~Sprite();
	virtual HRESULT Initialize();
	virtual void InitVertex();
	virtual void InitIndex();
	void SetSize(UINT width, UINT height) { imgSize_ = { width,height }; }
	void SetSize(XMUINT2 size) { imgSize_ = size; }

	void Draw(Transform& transform);
};

