#pragma once
class State
{
public:
	State();
	virtual ~State()=0;
	virtual void Initialize()=0;
	virtual void Update()=0;
};

