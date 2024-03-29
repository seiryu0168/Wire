#pragma once
#include"Engine/GameObject/GameObject.h"
#include"Engine/SceneManager.h"
#include"UIManager.h"
#include"EnemyManager.h"
#include"ItemManager.h"
#include"SetObject.h"
#include<list>
class Player;
class Enemy;
class ObjectSetter : public GameObject
{
private:
    EnemyManager EManager_;
    UIManager UManager_;
    ItemManager IManager_;
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
#ifdef _DEBUG
        void DebugCommand();
#endif // _DEBUG


        void GetEnemyList(std::list<Enemy*>* list);      //プレイヤーが認識してるエネミーのリストを返す
        void SetStageNum(int stageNum);
        std::string GetSceneName() { return sceneName_; };
        int GetEnemyCount();
};

