#pragma once
#include "GameObject/GameObject.h"
enum class SCENE_ID
{
	SCENE_ID_TITLE = 0,
	SCENE_ID_PLAY,
	SCENE_ID_RESULT,
	SCENE_ID_TUTORIAL,
	SCENE_ID_TEST

};

class SceneManager : public GameObject
{
	SCENE_ID currentSceneID_;
	SCENE_ID nextSceneID_;
	UINT countDown_;
public:
	SceneManager(GameObject* parent);

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void ChangeScene(SCENE_ID sceneID, UINT count = 0);
	int GetCountDown() { return countDown_; }
	SCENE_ID GetCurrentSceneID() { return currentSceneID_; }
	SCENE_ID GetNextSceneID() { return nextSceneID_; }
	GameObject* CurrentScene();
};

