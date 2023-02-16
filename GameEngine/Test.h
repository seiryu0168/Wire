#pragma once
#include "Engine/GameObject/GameObject.h"
#include"Engine/DirectX_11/Fbx.h"
#include"Engine/DirectX_11/BillBoard.h"
#include"Engine/DirectX_11/Particle.h"
class Test : public GameObject
{
    int hModel_;
    //Particle* pParticle_;
public:
    //�R���X�g���N�^
    Test(GameObject* parent);

    //�f�X�g���N�^
    ~Test();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;
    void FixedUpdate() override;
    //�`��
    void Draw() override;

    //�J��
    void Release() override;

    void OnCollision(GameObject* pTarget) override;
};

