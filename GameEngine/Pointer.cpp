#include "Pointer.h"
#include"Engine/Model.h"
Pointer::Pointer(GameObject* parent)
	:GameObject(parent,"Pointer"),
	drawFlag_(false)
{

}

//デストラクタ
Pointer::~Pointer()
{

}

//初期化
void Pointer::Initialize()
{
	hModel_ = Model::Load("Assets\\Mark.fbx");
	assert(hModel_ >= 0);
}

//更新
void Pointer::Update()
{

}

void Pointer::FixedUpdate()
{

}
//描画
void Pointer::Draw()
{
	Model::SetTransform(hModel_, transform_);
	
	if (drawFlag_)
	{
		Model::Draw(hModel_);
	}
}

void Pointer::SetPointerPos(XMFLOAT3 position)
{
	transform_.position_ = position;
}

//開放
void Pointer::Release()
{

}