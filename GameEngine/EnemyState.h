#pragma once
//�X�e�[�^�X�̐e�N���X
template<class state>
class EnemyState
{
public:
	virtual ~EnemyState() {}
	//�|�C���^�[ (*)
	//�A�N�Z�X�L�� ->
	//���Z���ł���
	//���̂��Ȃ���������Ȃ�
	//�n��������݂�Ə����������邩������Ȃ�

	//�Q��(&)
	//���Z�s��
	//���̂��K������
	//�n��������͎Q�Ƃ��R�s�[���킩��Ȃ�
	//���ʂȗ��R���Ȃ����const�Q�Ƃ��g��
	virtual void Init(state& enemy) {}
	virtual void Update(state& enemy) {}
};

