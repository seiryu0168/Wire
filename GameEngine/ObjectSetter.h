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
        //�R���X�g���N�^
        ObjectSetter(GameObject* parent);

        //�f�X�g���N�^
        ~ObjectSetter();

        //������
        void Initialize() override;

        //�X�V
        void Update() override;

       //�`��
        void Draw() override;

        void Release() override;

        std::list<Enemy*>* GetEnemyList();      //�v���C���[���F�����Ă�G�l�~�[�̃��X�g��Ԃ�

};

