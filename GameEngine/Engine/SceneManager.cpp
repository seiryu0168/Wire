#include"GameObject/RootJob.h"
#include "SceneManager.h"
#include"../TestScene.h"
#include"../TitleScene.h"
#include"../StageSelectScene.h"
#include"../PlayScene.h"
#include"../TutorialScene.h"
#include"../ResultScene.h"
#include"ResourceManager/Model.h"
#include"ResourceManager/ImageManager.h"


SceneManager::SceneManager(GameObject* parent) 
	: GameObject(parent, "SceneManager"),
	currentSceneID_(SCENE_ID::SCENE_ID_TITLE),
	nextSceneID_(SCENE_ID::SCENE_ID_TITLE),
	isSceneChange_(false)
{
	currentSceneID_ = SCENE_ID::SCENE_ID_TITLE;
	nextSceneID_ = currentSceneID_;

}

void SceneManager::Initialize()
{
	Instantiate<TitleScene>(this);
}

void SceneManager::Update()
{
	if (/*currentSceneID_ != nextSceneID_*/isSceneChange_&&countDown_ == 0)
	{
		KillAllChildren();
		ModelManager::AllDeleteModelNum();
		ModelManager::Release();
		ImageManager::AllRelease();

		SceneBase* p = nullptr;
		switch ((SCENE_ID)nextSceneID_)
		{
		case SCENE_ID::SCENE_ID_TITLE:p = Instantiate<TitleScene>(this); break;
		case SCENE_ID::SCENE_ID_SELECT:p = Instantiate<StageSelectScene>(this); break;
		case SCENE_ID::SCENE_ID_PLAY:p = Instantiate<PlayScene>(this); break;
		case SCENE_ID::SCENE_ID_RESULT:p = Instantiate<ResultScene>(this); break;
		case SCENE_ID::SCENE_ID_TUTORIAL:p = Instantiate<TutorialScene>(this); break;
		}

		isSceneChange_ = false;
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
	isSceneChange_ = true;
}
