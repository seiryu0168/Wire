#pragma once
#include"Engine/GameObject.h"
#include<list>
class Player;
class Enemy;
class ObjectSetter : public GameObject
{
private:
    std::list<Enemy*> enemys_;
    bool bossSpawn_;
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

        void TitleUpdate();

        void PlayUpdate();

        void GetEnemyList(std::list<Enemy*>* list);      //�v���C���[���F�����Ă�G�l�~�[�̃��X�g��Ԃ�

};

