#pragma once
#include"Engine/GameObject/GameObject.h"
#include"Engine/SceneManager.h"
#include"SetObject.h"
#include<list>
class Player;
class Enemy;
class ObjectSetter : public GameObject
{
private:
    std::list<Enemy*> enemys_;
    Player* pPlayer_;
    int countDown_;
    bool bossSpawn_;
    int hPict_;
    bool resultFrag_;
    std::string sceneName_;
    SetObject* set_;
    SceneManager* pManager_;
    SCENE_ID nowSceneID_;
public:
        //コンストラクタ
        ObjectSetter(GameObject* parent);

        //デストラクタ
        ~ObjectSetter();

        //初期化
        void Initialize() override;

        //更新
        void Update() override;

       //描画
        void Draw() override;
        void ThirdDraw() override;

        void Release() override;

        void TitleUpdate();

        void PlayUpdate();

        void TutorialUpdate();

        void BlackOutUpdate();

        void GetEnemyList(std::list<Enemy*>* list);      //プレイヤーが認識してるエネミーのリストを返す

        std::string GetSceneName() { return sceneName_; };
        int GetEnemyCount();
};

