#include "PlayScene.h"
#include"Player.h"
#include"ObjectSetter.h"
#include"Test.h"
// コンストラクタ
PlayScene::PlayScene(GameObject * parent)
	: SceneBase(parent, "PlayScene")
{
	Instantiate<ObjectSetter>(this);
}

//初期化
void PlayScene::Initialize()
{
}

//更新
void PlayScene::Update()
{
}

//描画
void PlayScene::Draw()
{
}

//開放
void PlayScene::Release()
{
	int a = 0;
}