#pragma once
#include"SceneBase.h"
#include"Engine/DirectX_11/LineParticle.h"
class TestScene : public SceneBase
{
	LineParticle* pLine_;
public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	TestScene(GameObject* parent);

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

