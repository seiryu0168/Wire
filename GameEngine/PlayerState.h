#pragma once

template<class C>
class PlayerState
{
public:
	virtual ~PlayerState() {}
	
	//������(��Ԃ��ς�邲�ƂɌĂ΂��)
	virtual void Init(C& player) {}

	virtual void Update(C& player) {}
};