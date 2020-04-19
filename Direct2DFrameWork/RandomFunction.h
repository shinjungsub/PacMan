#pragma once
#include "SingletonBase.h"

class RandomFunction : public SingletonBase <RandomFunction>
{
public:
	RandomFunction();
	~RandomFunction() {}
	//GetInt
	int Range(int num);
	//GetFromIntTo
	int Range(int fromNum, int toNum);
	//GetFloat
	float Range(float num);
	//GetFromFloatTo
	float Range(float fromNum, float toNum);
};

