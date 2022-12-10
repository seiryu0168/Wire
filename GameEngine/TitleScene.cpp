#include "TitleScene.h"
#include"Engine/Model.h"
#include"Engine/Fbx.h"
#include"Player.h"
#include"EnemyNormal.h"
#include"EnemyTurret.h"
#include"SetObject.h"
#include"Test.h"
#include"Stage1.h"
#include"StateList.h"

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
	//Instantiate<SetObject>(this);
	State::StateCreate();
	Instantiate<Stage1>(this);
	Instantiate<Player>(this);
	for (int i = 0; i < 11; i++)
	{
		Instantiate<EnemyNormal>(this);
		Instantiate<EnemyTurret>(this);
	}
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