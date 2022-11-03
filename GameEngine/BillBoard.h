#pragma once
#include<string>
#include"Engine/Direct3D.h"
#include"Engine/Texture.h"
struct CONSTANT_BUFFER
{
	XMMATRIX matWVP;
	XMFLOAT4 color;
};

struct VERTEX
{
	XMFLOAT3 position;
	XMFLOAT3 uv;
};
class BillBoard
{
	ID3D11Buffer* pVertexBuffer_;
	ID3D11Buffer* pConstantBuffer_;
	ID3D11Buffer* pIndexBuffer_;

	Texture* pTexture_;
public:
	BillBoard();
	~BillBoard();

	//ÉçÅ[Éh
	HRESULT Load(std::string fileName);
	//ï`âÊ
	void Draw(XMMATRIX matW,XMFLOAT4 col);
	void Release();
};

