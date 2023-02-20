#pragma once
#include"Engine/GameObject/GameObject.h"
#include"Engine/SceneManager.h"
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
    SceneManager* pManager_;
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

        void Release() override;

        void TitleUpdate();

        void PlayUpdate();

        void GetEnemyList(std::list<Enemy*>* list);      //プレイヤーが認識してるエネミーのリストを返す

};

