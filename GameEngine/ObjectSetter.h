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

