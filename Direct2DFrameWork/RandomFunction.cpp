#include "stdafx.h"
#include "RandomFunction.h"

RandomFunction::RandomFunction()
{
	//�����õ� �ʱ�ȭ
	//srand(time(NULL));
	srand(GetTickCount());
}

//0 ~ (num-1) ������ ������
int RandomFunction::Range(int num)
{
	return rand() % num;
}

//fromNum ~ toNum ������ ������
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
