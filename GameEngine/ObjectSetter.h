#pragma once
#include"Engine/GameObject.h"
#include<list>
class Player;
class Enemy;
class ObjectSetter : public GameObject
{
private:
    std::list<Enemy*> enemys_;
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

};

