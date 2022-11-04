#pragma once
#include"Engine/GameObject.h"

struct EmitterData
{
    std::string textureFileName;    //貼るテクスチャのファイル名
    XMFLOAT3 position;              //エミッターの位置
    XMFLOAT3 positionErr;           //エミッターの誤差
    XMFLOAT3 dir;                   //パーティクルの発射方向
    XMFLOAT3 dirErr;                //パーティクルの発射方向の誤差
    float speed;                    //1フレームあたりの移動量
    float speedErr;                 //移動量の誤差
    float acceleration;             //加速度
    float gravity;                  //重力
    XMFLOAT4 color;                 //色 : RGBA 0～１
    XMFLOAT4 deltaColor;            //色の変化量
    XMFLOAT2 size;                  //サイズ
    XMFLOAT2 sizeErr;               //サイズの誤差
    XMFLOAT2 scale;                 //1フレームあたりの拡大率
    float lifTime;                  //パーティクルの寿命
    
    int delay;                      //パーティクルを出す間隔のフレーム数
    int number;                     //1回に出すパーティクルの数

//Emitter初期化
        EmitterData()
        {
            textureFileName = "";
            position = positionErr = dir = dirErr = { 0,0,0 };
            speed = 0.0f;
            acceleration = 1.0f;
            gravity = 0.0f;
            color = { 1,1,1,1 };
            deltaColor = { 0,0,0,0 };
            size = scale = { 1.0f,1.0f };
            lifTime = 10.0f;
        }

};
class Particle : public GameObject
{
private:
    int hModel_;
    float lifeTime_;
     XMFLOAT3 particleScale_;
     XMFLOAT3 particleRotate_;

public:
    Particle(GameObject* parent);

    //デストラクタ
    ~Particle();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    void FixedUpdate() override;
    //描画
    void Draw() override;

    //開放
    void Release() override;
};

