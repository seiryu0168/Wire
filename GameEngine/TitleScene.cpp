#include "TitleScene.h"
#include"Engine/DirectX_11/Sprite.h"
#include"ObjectSetter.h"
#include"Engine/ResourceManager/ImageManager.h"

TitleScene::TitleScene(GameObject* parent)
	: SceneBase(parent, "TitleScene")
{
	Instantiate<ObjectSetter>(this);
}

//������
void TitleScene::Initialize()
{
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