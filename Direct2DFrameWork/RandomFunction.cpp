#include "stdafx.h"
#include "RandomFunction.h"

RandomFunction::RandomFunction()
{
	//랜덤시드 초기화
	//srand(time(NULL));
	srand(GetTickCount());
}

//0 ~ (num-1) 사이의 랜덤값
int RandomFunction::Range(int num)
{
	return rand() % num;
}

//fromNum ~ toNum 사이의 랜덤값
int RandomFunction::Range(int fromNum, int toNum)
{
	return rand() % (toNum - fromNum + 1) + fromNum;
}

float RandomFunction::Range(float num)
{
	return ((float)rand() / (float)RAND_MAX) *num;
}

float RandomFunction::Range(float fromNum, float toNum)
{
	float rnd = (float)rand() / (float)RAND_MAX;
	return (rnd * (toNum - fromNum) + fromNum);
}
