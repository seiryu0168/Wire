#pragma once
#include<vector>
#include"Engine/DirectX_11/Direct3D.h"
#include<string>
class PlayScreen
{
private:
	int hPict_[4];

public:
	PlayScreen();
	~PlayScreen();
	void Update(XMFLOAT2 dir);

};

