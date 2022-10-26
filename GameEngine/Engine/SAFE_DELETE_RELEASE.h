#pragma once
#include<DirectXMath.h>
using namespace DirectX;
#define SAFE_DELETE(p) if(p != nullptr){delete p;p=nullptr;}
#define SAFE_DELETE_ARRAY(p) if (p != nullptr) {delete[] p;p = nullptr;}
#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}
#define M_PI 3.14159265359

//float selfVector3Dot(XMVECTOR vec1, XMVECTOR vec2)
//{
//	XMFLOAT3 fVec1;
//	XMFLOAT3 fVec2;
//
//	XMStoreFloat3(&fVec1, XMVector3Normalize(vec1));
//	XMStoreFloat3(&fVec2, XMVector3Normalize(vec2));
//
//	return ((fVec1.x * fVec2.x) + (fVec1.y * fVec2.y) + (fVec1.z * fVec2.z));
//}