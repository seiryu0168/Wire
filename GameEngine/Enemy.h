#pragma once
#include"Engine/GameObject.h"
class Enemy : public GameObject
{
protected:
    //コンストラクタ
    Enemy(GameObject* parent,std::string name);

    //デストラクタ
    ~Enemy();
};

