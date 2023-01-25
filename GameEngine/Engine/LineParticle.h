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

	//���_���
	struct VERTEX
	{
		XMFLOAT3 position;
		XMFLOAT3 uv;
	};

	float WIDTH_;//���C���p�[�e�B�N���̕�
	int LENGTH_;//���C���p�[�e�B�N���̃|�W�V�������L�������
	float tipWidth_;

	ID3D11Buffer* pVertexBuffer_;
	ID3D11Buffer* pConstantBuffer_;

	Texture* pTexture_;
	std::list<XMFLOAT3> positionList_;
public:
	LineParticle();
	LineParticle(float width,int length,float tipWidth);
	//���݈ʒu���L�� : pos
	void AddPosition(XMFLOAT3 pos);
	HRESULT CreateMeshPype(std::list<XMFLOAT3>* pList,int splt);
	HRESULT CreateMeshPlate(std::list<XMFLOAT3>* pList, int splt);
	HRESULT Load(std::string fileName);
	void Draw();
	void SetLineParameter(float width, int length,float tipWidth=0);
	void DeleteLine();
	void Release();



};

