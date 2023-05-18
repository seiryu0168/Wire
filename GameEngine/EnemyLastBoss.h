#pragma once
#include"Enemy.h"
class EnemyLastBoss : public Enemy
{
private:
	EnemyState<EnemyLastBoss>* pState_;

	class StateFirstMode : public EnemyState<EnemyLastBoss>
	{

		int hModel_;
	public:
		static StateFirstMode* GetInstance()
		{
			static StateFirstMode* instance = 0;
			if (instance == nullptr)
			{
				instance = new StateFirstMode;
			}
			return instance;
		}
		void Init(EnemyLastBoss& enemy) override;
		void Update(EnemyLastBoss& enemy) override;

	};
	void ChangeState(EnemyState<EnemyLastBoss>* state);

public:
	EnemyLastBoss(GameObject* parent);
	~EnemyLastBoss();
	void Initialize() override;
	void Update() override;
	void Draw() override;
};

