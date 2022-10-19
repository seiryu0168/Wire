#pragma once
#include<DirectXMath.h>

using namespace DirectX;
class Transform
{
public:
	XMMATRIX matTranslate_;	//移動行列
	XMMATRIX matRotate_;	//回転行列	
	XMMATRIX matScale_;	//拡大行列

	XMFLOAT3 position_;	//位置
	XMFLOAT3 rotate_;	//向き
	XMFLOAT3 scale_;	//拡大率
	Transform* pParent_;//親の情報

	//コンストラクタ
	Transform();

	//デストラクタ
	~Transform();

	//各行列の計算
	void Calclation();

	//回転行列と拡縮の逆行列の計算
	XMMATRIX GetNormalMatrix();
	//ワールド行列を取得
	XMMATRIX GetWorldMatrix();
	XMMATRIX GetRotateMatrix();
	XMMATRIX GetScaleMatrix();
	
	static XMFLOAT3 Float3Add(XMFLOAT3 add1, XMFLOAT3 add2)
	{
		return XMFLOAT3(add1.x + add2.x, add1.y + add2.y, add1.z + add2.z);
	}
};

