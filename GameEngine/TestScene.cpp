#include "TestScene.h"

//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
TestScene::TestScene(GameObject* parent)
	:SceneBase(parent,"TestScene")
{

}
//������
void TestScene::Initialize()
{
	pLine_ = new LineParticle;
	pLine_->Load("Assets\\Effect01.png");
}

//�X�V
void TestScene::Update()
{
	pLine_->AddPosition(transform_.position_);
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