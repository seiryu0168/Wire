#include "TestScene.h"

//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
TestScene::TestScene(GameObject* parent)
	:SceneBase(parent,"TestScene"),
	pLine_(nullptr)
{

}
//������
void TestScene::Initialize()
{
}

//�X�V
void TestScene::Update()
{
}
void TestScene::FixedUpdate()
{

}

//�`��
void TestScene::Draw()
{
	//pLine_->Draw();
}

//�J��
void TestScene::Release()
{

}