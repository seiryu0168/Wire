#pragma once
#include"SceneBase.h"

class TitleScene : public SceneBase
{
	
	
public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	TitleScene(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;
	void FixedUpdate() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;
};

