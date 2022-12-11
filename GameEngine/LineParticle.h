#pragma once
#include"Engine/Direct3D.h"
#include"Engine/Texture.h"
#include"DirectXMath.h"
#include<list>



class LineParticle
{
private:
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
	float WIDTH_;
	float LENGTH_;

	ID3D11Buffer* pVertexBuffer_;
	ID3D11Buffer* pConstantBuffer_;

	Texture* pTexture_;
	std::list<XMFLOAT3> positionList_;
public:
	LineParticle();

	//åªç›à íuÇãLâØ : pos
	void AddPosition(XMFLOAT3 pos);

	HRESULT Load(std::string fileName);
	void Draw();
	void Release();



};

