#include"RootJob.h"
#include "SceneManager.h"
#include"../TestScene.h"
#include"../PlayScene.h"
#include"../TitleScene.h"
#include"Model.h"
#include"ImageManager.h"


SceneManager::SceneManager(GameObject* parent) 
	: GameObject(parent, "SceneManager"),currentSceneID_(0),nextSceneID_(0)
{

}

void SceneManager::Initialize()
{
	currentSceneID_ = (int)SCENE_ID::SCENE_ID_TITLE;
	nextSceneID_ = currentSceneID_;
	Instantiate<TitleScene>(this);
}

void SceneManager::Update()
{
	if (currentSceneID_ != nextSceneID_)
	{
		KillAllChildren();
		ModelManager::Release();
		ImageManager::AllRelease();


		switch ((SCENE_ID)nextSceneID_)
		{
		case SCENE_ID::SCENE_ID_TITLE: Instantiate<TitleScene>(this); break;
		case SCENE_ID::SCENE_ID_PLAY: Instantiate<PlayScene>(this); break;
		case SCENE_ID::SCENE_ID_TEST: Instantiate<TestScene>(this); break;
		}
		currentSceneID_ = nextSceneID_;
	}
}

void SceneManager::Draw()
{

}

void SceneManager::Release()
{

}
void SceneManager::ChangeScene(int sceneID)
{
	nextSceneID_ = sceneID;
}
