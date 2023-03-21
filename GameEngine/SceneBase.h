#pragma once
#include"Engine/GameObject/GameObject.h"
#include<vector>
class SceneBase : public GameObject
{
private:
    struct objectData
    {
        std::string objectClassName;
        Transform objectTransform;

    };
    std::vector<objectData> objectDataList_;
public:
	SceneBase(GameObject* parent, std::string sceneName);
	virtual ~SceneBase() {};

	void Load();
    void OutData();
	void SetData();
};

