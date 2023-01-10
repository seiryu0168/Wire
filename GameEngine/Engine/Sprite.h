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

	void ToPipeLine(Transform& transform);
	void bufferSet();
	void Release();

public:
	Sprite();
	~Sprite();
	virtual HRESULT Initialize();
	virtual void InitVertex();
	virtual void InitIndex();
	HRESULT Load(std::string fileName);
	
	void SetSize(UINT width, UINT height) { imgSize_ = { width,height }; }

	void Draw(Transform& transform,RECT rect,float alpha);
};

