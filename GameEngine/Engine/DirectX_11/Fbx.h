#pragma once

#include <d3d11.h>
#include <fbxsdk.h>
#include <string>
#include"Direct3D.h"
#include<list>
#include "../GameObject/Transform.h"

#pragma comment(lib, "LibFbxSDK-MT.lib")
#pragma comment(lib, "LibXml2-MT.lib")
#pragma comment(lib, "zlib-MT.lib")

class Texture;

struct RayCastData
{
	XMFLOAT3 start;
	XMFLOAT3 dir;
	float dist;
	XMVECTOR hitPos;
	XMVECTOR normal;
	BOOL hit;
	float distLimit;
	std::list<int> hitModelList;

	RayCastData() :start(XMFLOAT3(0, 0, 0)),
		dir(XMFLOAT3(0, 0, 0)),
		dist(9999.0f),
		hit(false),
		hitPos(XMVectorSet(0, 0, 0, 0)),
		normal(XMVectorSet(0, 0, 0, 0)),
		distLimit(9999.0f){}
};

	 class FbxParts;
class Fbx
{
	std::vector<FbxParts*> parts_;
	int vertexCount_;		//���_��
	int polygonCount_;		//�|���S����
	int materialCount_;		//�}�e���A����

	FbxManager* pFbxManager_;
	FbxScene* pFbxScene_;
	FbxTime::EMode frameRate_;
	float animSpeed_;
	int startFrame_;
	int endFrame_;

	HRESULT CheckNode(FbxNode* pNode, std::vector<FbxParts*>* pPartsList);
public:

	Fbx();
	~Fbx();
	HRESULT Load(std::string fileName);
	void RayCast(RayCastData& ray,Transform& transform);
	XMFLOAT3 GetBonePosition(std::string boneName);
	void    Draw(Transform& transform, SHADER_TYPE shaderType,int frame);
	void	DrawOutLine(Transform& transform, int frame, XMFLOAT4 lineColor = {0,0,0,1});
	void	DrawToon(Transform& transform, bool isOutLine, int frame);
	void    Release();
};