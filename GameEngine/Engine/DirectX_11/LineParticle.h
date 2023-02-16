#pragma once
#include"Direct3D.h"
#include"Texture.h"
#include"DirectXMath.h"
#include"../GameObject/Transform.h"
#include<list>

class LineParticle
{
	enum class LineMode
	{
		LINE_DEFAULT=0,
		LINE_CROSS,
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
	size_t LENGTH_;//���C���p�[�e�B�N���̃|�W�V�������L�������
	float tipWidth_;
	//int* index_;

	ID3D11Buffer* pVertexBuffer_;
	ID3D11Buffer* pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;

	Texture* pTexture_;
	std::vector<int> indexList;
	std::list<XMFLOAT3> positionList_;
public:
	LineParticle();
	LineParticle(float width,int length,float tipWidth);
	//���݈ʒu���L�� : pos
	void AddPosition(XMFLOAT3 pos);
	HRESULT CreateMeshPype(std::list<XMFLOAT3>* pList);
	HRESULT CreateMeshPlate(std::list<XMFLOAT3>* pList);
	HRESULT Load(std::string fileName);
	void SetIndex();
	void Draw(Transform* transform);
	void SetLineParameter(float width, int length,float tipWidth=0);
	void DeleteLine();
	void Release();



};

