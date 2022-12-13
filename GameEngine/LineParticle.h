#pragma once
#include"Engine/Direct3D.h"
#include"Engine/Texture.h"
#include"DirectXMath.h"
//#include"Engine/BillBoard.h"
#include<list>



class LineParticle
{
private:
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;
		XMFLOAT4	color;
	};

	//頂点情報
	struct VERTEX
	{
		XMFLOAT3 position;
		XMFLOAT3 uv;
	};

	float WIDTH_;//ラインパーティクルの幅
	int LENGTH_;//ラインパーティクルのポジションを記憶する量

	ID3D11Buffer* pVertexBuffer_;
	ID3D11Buffer* pConstantBuffer_;

	Texture* pTexture_;
	std::list<XMFLOAT3> positionList_;
public:
	LineParticle();
	LineParticle(float width,int length);
	//現在位置を記憶 : pos
	void AddPosition(XMFLOAT3 pos);

	HRESULT Load(std::string fileName);
	void Draw();
	void Release();
	void SetLineParameter(float width, int length);



};

