#pragma once
#include"Engine/GameObject/GameObject.h"
#include"Engine/SceneManager.h"
#include"SetObject.h"
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
    bool resultFrag_;
    std::string sceneName_;
    SetObject* set_;
    SceneManager* pManager_;
    SCENE_ID nowSceneID_;
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
        void ThirdDraw() override;

        void Release() override;

        void TitleUpdate();

        void PlayUpdate();

        void TutorialUpdate();

        void BlackOutUpdate();

        void GetEnemyList(std::list<Enemy*>* list);      //�v���C���[���F�����Ă�G�l�~�[�̃��X�g��Ԃ�

        std::string GetSceneName() { return sceneName_; };
        int GetEnemyCount();
};

