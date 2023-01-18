#pragma once
//ステータスの親クラス
template<class state>
class EnemyState
{
public:
	virtual ~EnemyState() {}
	//ポインター (*)
	//アクセス記号 ->
	//演算ができる
	//実体がないかもしれない
	//渡す側からみると書き換えられるかもしれない

	//参照(&)
	//演算不可
	//実体が必ずある
	//渡す側からは参照かコピーかわからない
	//特別な理由がなければconst参照を使う
	virtual void Init(state& enemy) {}
	virtual void Update(state& enemy) {}
};

