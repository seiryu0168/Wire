#include "SelectScene.h"
#include"ObjectSetter.h"
SelectScene::SelectScene(GameObject* parent)
	:SceneBase(parent,"SelectScene")
{
	Instantiate<ObjectSetter>(this);
}

SelectScene::~SelectScene()
{
}

void SelectScene::Initialize()
{

}

void SelectScene::Release()
{
}
