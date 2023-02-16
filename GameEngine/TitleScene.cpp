#include "TitleScene.h"
#include"Engine/DirectX_11/Sprite.h"
#include"ObjectSetter.h"
#include"Engine/ResourceManager/ImageManager.h"

TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene")
{
}

//������
void TitleScene::Initialize()
{
	Instantiate<ObjectSetter>(this);
}

//�X�V
void TitleScene::Update()
{

	
}

void TitleScene::FixedUpdate()
{

}

//�`��
void TitleScene::Draw()
{
}

//�J��
void TitleScene::Release()
{
}