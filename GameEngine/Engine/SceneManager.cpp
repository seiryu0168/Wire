#include"GameObject/RootJob.h"
#include "SceneManager.h"
#include"../TestScene.h"
#include"../PlayScene.h"
#include"../TitleScene.h"
#include"../ResultScene.h"
#include"../TutorialScene.h"
#include"ResourceManager/Model.h"
#include"ResourceManager/ImageManager.h"


SceneManager::SceneManager(GameObject* parent) 
	: GameObject(parent, "SceneManager"),currentSceneID_(SCENE_ID::SCENE_ID_TITLE),nextSceneID_(SCENE_ID::SCENE_ID_TITLE)
{

}

void SceneManager::Initialize()
{
	currentSceneID_ = SCENE_ID::SCENE_ID_TITLE;
	nextSceneID_ = currentSceneID_;
	Instantiate<TitleScene>(this);
}

void SceneManager::Update()
{
	if (currentSceneID_ != nextSceneID_&&countDown_ == 0)
	{
		KillAllChildren();
		ModelManager::AllDeleteModelNum();
		ModelManager::Release();
		ImageManager::AllRelease();

		SceneBase* p = nullptr;
		switch ((SCENE_ID)nextSceneID_)
		{
		case SCENE_ID::SCENE_ID_TITLE:p = Instantiate<TitleScene>(this); break;
		case SCENE_ID::SCENE_ID_PLAY:p = Instantiate<PlayScene>(this); break;
		case SCENE_ID::SCENE_ID_RESULT:p = Instantiate<ResultScene>(this); break;
		case SCENE_ID::SCENE_ID_TUTORIAL:p = Instantiate<TutorialScene>(this); break;
		}

		currentSceneID_ = nextSceneID_;
	}
	countDown_--;
}

void SceneManager::Draw()
{

}

void SceneManager::Release()
{

}
void SceneManager::ChangeScene(SCENE_ID sceneID,UINT count)
{
	nextSceneID_ = sceneID;
	countDown_ = count;
}
