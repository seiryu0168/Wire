#pragma once
#include "GameObject/GameObject.h"
enum class SCENE_ID
{
	SCENE_ID_TITLE = 0,
	SCENE_ID_PLAY,
	SCENE_ID_RESULT,
	SCENE_ID_TEST

};

class SceneManager : public GameObject
{
	int currentSceneID_;
	int nextSceneID_;
public:
	SceneManager(GameObject* parent);

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void ChangeScene(int sceneID);
	int GetCurrentSceneID() { return currentSceneID_; }
	GameObject* CurrentScene();
};

