#pragma once
#include"Direct3D.h"
#include"Texture.h"
#include"DirectXMath.h"
#include<list>

class LineParticle
{
	enum LineMode
	{
		DEFAULT=0,
		BILLBOARD,
	};

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
	float tipWidth_;

	ID3D11Buffer* pVertexBuffer_;
	ID3D11Buffer* pConstantBuffer_;

	Texture* pTexture_;
	std::list<XMFLOAT3> positionList_;
public:
	LineParticle();
	LineParticle(float width,int length,float tipWidth);
	//現在位置を記憶 : pos
	void AddPosition(XMFLOAT3 pos);

	HRESULT Load(std::string fileName);
	void Draw();
	void SetLineParameter(float width, int length,float tipWidth=0);
	void DeleteLine();
	void Release();



};

