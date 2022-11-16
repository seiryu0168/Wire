#pragma once

namespace EngineTime
{
//public:
//	EngineTime();
//	~EngineTime();

	unsigned long GetTime();
	float GetDeltaTime();
	float GetTimeScale();
	void SetTimeScale(float scale);
	void SetTime();
};
