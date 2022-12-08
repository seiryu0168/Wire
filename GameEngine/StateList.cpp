#include"StateList.h"

void State::StateCreate()
{
	search = new StateSearch;
	chase = new StateChase;
}