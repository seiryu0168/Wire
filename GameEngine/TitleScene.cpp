#include "TitleScene.h"
#include"Engine/Model.h"
#include"Engine/Fbx.h"
#include"Player.h"
#include"Enemy_normal.h"
#include"Test.h"
#include"Stage1.h"

TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene")
{
}

//初期化
void TitleScene::Initialize()
{
	//オブジェクト設置するクラスを作り、そのクラスの中で各オブジェクトを生成する
	//生成時にここのクラスを親に設定すればOK
	//オブジェクト設置クラスでエネミーのパラメータを設定するようにする
	 
	//feildって名前にしといたほうがいい
	Instantiate<Stage1>(this);
	Instantiate<Player>(this);
	Instantiate<Enemy_Normal>(this);
	Instantiate<Test>(this);
}

//更新
void TitleScene::Update()
{
	
}

void TitleScene::FixedUpdate()
{

}

//描画
void TitleScene::Draw()
{
}

//開放
void TitleScene::Release()
{
}