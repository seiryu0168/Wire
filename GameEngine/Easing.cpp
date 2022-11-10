#include "Easing.h"
#include<iostream>
#include"Engine/SAFE_DELETE_RELEASE.h"

void Clamp(float& number, float min=0.0f, float max=1.0f);
float Easing::EaseLinear(float time)
{
	Clamp(time);
	return time;
}
float Easing::EaseInSine(float time)
{
	Clamp(time);
	return (1.0f - std::cos((time*M_PI)/2.0f));
}
float Easing::EaseOutSine(float time)
{
	Clamp(time);
	return (1.0f - std::sin((time * M_PI) / 2.0f));
}
float Easing::EaseINOutSine(float time)
{
	Clamp(time);
	return (-(std::cos(M_PI * time) - 1.0f) / 2.0f);

}

float Easing::EaseInQuad(float time)
{
	Clamp(time);
	return (time * time);
}
float Easing::EaseOutQuad(float time)
{
	Clamp(time);
	return (1.0f - (1.0f - time) * (1.0f - time));
}

float Easing::EaseINOutQuad(float time)
{
	Clamp(time);
	return (time < 0.5f ? 2 * time * time : 1 - std::pow((-2 * time + 2), 2) / 2);
}

void Clamp(float &number,float min,float max)
{
	if (number > max)
	{
		 number = max;
	}

	if (number < min)
	{
		number = min;
	}
}