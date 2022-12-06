#include "Transform.h"

//コンストラクタ
Transform::Transform()
{
	matTranslate_=XMMatrixIdentity();
	matRotate_=XMMatrixIdentity();
	matScale_=XMMatrixIdentity();
	position_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotate_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale_ = XMFLOAT3(1.0f, 1.0f, 1.0f);
	pParent_ = nullptr;
}

//デストラクタ
Transform::~Transform()
{

}

//各行列の計算
void Transform::Calclation()
{
	//移動行列
	matTranslate_ = XMMatrixTranslation(position_.x, position_.y, position_.z);
	
	//回転行列
	matRotate_ = XMMatrixRotationZ(XMConvertToRadians(rotate_.z))
			   * XMMatrixRotationX(XMConvertToRadians(rotate_.x))
			   * XMMatrixRotationY(XMConvertToRadians(rotate_.y));
	
	//拡大行列
	matScale_ = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
}

//逆行列の計算
XMMATRIX Transform::GetNormalMatrix()
{
	return matRotate_ * XMMatrixInverse(nullptr, matScale_);
}

//ワールド行列を取得
XMMATRIX Transform::GetWorldMatrix()
{
	Calclation();
	if (pParent_)
	{
		return matScale_ * matRotate_ * matTranslate_ * pParent_->GetWorldMatrix();
	}
	return matScale_ * matRotate_ * matTranslate_;

}

XMMATRIX Transform::GetRotateMatrix()
{
	return matRotate_;
}
XMMATRIX Transform::GetScaleMatrix()
{
	return matScale_;
}