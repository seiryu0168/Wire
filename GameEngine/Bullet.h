#pragma once
#include "Engine/GameObject/GameObject.h"
#include"Engine/DirectX_11/BillBoard.h"

class Particle;
class Bullet : public GameObject
{
private:
    int hModel_;
    int hAudio_;
    int life_;
    float speed_;

    BillBoard* pBill_;
    Particle* pParticle_;
    XMVECTOR dir_;

public:
    //�R���X�g���N�^
    Bullet(GameObject* parent);

    //�f�X�g���N�^
    ~Bullet();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    void SetDir(XMVECTOR dir) { dir_ = dir; }
    //�`��
    void Draw() override;
    
    //�Փˎ�
    void OnCollision(GameObject* target) override;

    void Release() override;
};

