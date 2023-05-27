#include "TestScene.h"

//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
TestScene::TestScene(GameObject* parent)
	:SceneBase(parent,"TestScene")
{

}
//初期化
void TestScene::Initialize()
{
}

//更新
void TestScene::Update()
{
}
void TestScene::FixedUpdate()
{

}

//描画
void TestScene::Draw()
{
	//pLine_->Draw();
}

//開放
void TestScene::Release()
{

}