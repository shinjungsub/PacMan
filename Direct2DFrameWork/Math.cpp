#include "stdafx.h"
#include "Math.h"

const float Math::PI = 3.14159265f;
const float Math::Epsilon = 1E-6f;
const int Math::IntMin = -2147483647;
const int Math::IntMax = 2147483647;
const float Math::FloatMin = -3.402823E+38f;
const float Math::FloatMax = 3.402823E+38f;
/**********************************************************
## NegativeChecking ##
소수의 음수 양수를 판단해 ~1.f또는 1.f 반환
@@ float value : 값
**********************************************************/
float Math::NegativeChecking(float value)
{
	if (value < 0.0f)
		return -1.0f;
	else
		return 1.0f;
}

/**********************************************************
## ToRadian ##
디그리값을 라디안으로 변환해 반환
@@ float degree : 디그리값
**********************************************************/
float Math::ToRadian(float degree)
{
	return degree * Math::PI / 180.0f;
}
/**********************************************************
## ToDegree ##
라디안을 디그리로 변환해 반환
@@ float radian : 라디안 값
**********************************************************/
float Math::ToDegree(float radian)
{
	return radian * 180.0f / Math::PI;
}
/**********************************************************
## Random ##
@@ float r1 : 시작 값
@@ float r2 : 끝 값
**********************************************************/
float Math::Random(float r1, float r2)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = r2 - r1;
	float val = random * diff;

	return r1 + val;
}
/**********************************************************
## RandF ##
소수형 난수 반환
**********************************************************/
float Math::RandF()
{
	return (float)(rand()) / (float)RAND_MAX;
}
/**********************************************************
## Lerp ##
비율을 통해 두 값을 선형 보간해서 반환
@@ float val1 : 시작 값
@@ float val2 : 도달 값
@@ float amount : 두 값의 비율, 0.0f ~ 1.0f 사이의 값이 들어간다.
**********************************************************/
float Math::Lerp(float val1, float val2, float amount)
{
	return val1 + (val2 - val1) * amount;
}
/**********************************************************
## Clampf ##
어떠한 값이 min,max보다 크거나 작아지려고 하면 min또는max를 반환
@@ float value : 값
@@ float min : 최소 범위
@@ float max : 최대 범위
**********************************************************/
float Math::Clampf(float value, float min, float max)
{
	value = value >= max ? max : value;
	value = value <= min ? min : value;

	return value;
}
/**********************************************************
## Clamp ##
어떠한 값이 min,max보다 크거나 작아지려고 하면 min또는max를 반환
@@ int value : 값
@@ int min : 최소 범위
@@ int max : 최대 범위
**********************************************************/
int Math::Clamp(int value, int min, int max)
{
	value = value > max ? max : value;
	value = value < min ? min : value;

	return value;
}

/***********************************************************
## Modulo ##
어디에 쓰는지 기억 안남
***********************************************************/
float Math::Modulo(float val1, float val2)
{
	while (val1 - val2 >= 0)
		val1 -= val2;

	return val1;
}
/*********************************************************************
## InDistance ##
두 점 사이의 거리가 maxDistance를 넘어갔는지 확인 용도
sqrt 함수는 비용이 크기 때문에 제곱상태에서 확인
@@ vector2 start : 시작점
@@ vector2 end : 목표점
@@ float maxDistance : 확인하고 싶은 거리
**********************************************************************/
bool Math::InDistance(Vector2 start, Vector2 end, float maxDistance)
{
	float x = end.x - start.x;
	float y = end.y - start.y;

	if (x * x + y * y < maxDistance * maxDistance)
		return true;
	else
		return false;
}
/*********************************************************************
## GetDistance ##
두 점 사이의 거리를 구한다
@@ float startX : 시작점 X
@@ float startY : 시작점 y
@@ float endX : 목표점 x
@@ float endY : 목표점 y
**********************************************************************/
float Math::GetDistance(float startX, float startY, float endX, float endY)
{
	float x = endX - startX;
	float y = endY - startY;

	return sqrt(x * x + y * y);
}
/*******************************************************************************
## GetAngle ##
두 점 사이의 각을 구한다.
@@ float x1 : 시작점 X
@@ float y1 : 시작점 Y
@@ float x2 : 목표점 X
@@ float y2 : 목표점 Y
*******************************************************************************/
float Math::GetAngle(float x1, float y1, float x2, float y2)
{
	//x축과 y축으로의 길이를 구한다. 
	float x = x2 - x1;
	float y = y2 - y1;
	//피타고라스 정리로 대각선의 길이 구함
	float distance = sqrt((x * x) + (y * y));
	//삼각함수의 원리에 의해 cos(angle) = x / 빗변  == angle = acos(x / 빗변);
	float angle = acos(x / distance);
	//만약 구하려는 각이 뒤집어져 있는 상황이라면 
	if (y2 > y1)
	{
		//역으로 360에서 -한값을 구한다. 
		angle = Math::PI * 2.0f - angle;
		if (angle >= Math::PI * 2.0f)
			angle -= Math::PI * 2.0f;
	}

	return angle;
}

/*******************************************************************************
## Random ##
@@ int r1 : 시작 범위
@@ int r2 : 끝 범위
*******************************************************************************/
int Math::Random(int r1, int r2)
{
	return (int)(rand() % (r2 - r1 + 1)) + r1;
}

/*******************************************************************************
## EqualFloat ##
@@ float f1 : 첫번째 소수
@@ float f2 : 두번째 소수
*******************************************************************************/
bool Math::EqualFloat(float f1, float f2)
{
	//값의 차이를 절대값으로 만든 뒤 tiny float point보다 작거나 같으면 같은 것으로 본다
	if (fabsf(f1 - f2) <= FLT_EPSILON) return true;
	return false;
}

