#include "stdafx.h"
#include "Vector2.h"

/*******************************************************************************
## Normalize ##
현재 벡터를 정규화한 값을 반환
*******************************************************************************/
Vector2 Vector2::Normalize()
{
	Vector2 result;
	float size = 1;

	size = sqrtf(x * x + y * y);

	if (size != 0)
	{
		result.x = x / size;
		result.y = y / size;
	}
	else
	{
		result.x = 0;
		result.y = 0;
	}

	return result;
}
/*******************************************************************************
## StartDirVector ##
현재 벡터를 start에서 end 쪽을 가르키는 방향벡터로 설정한다.
@@ Vector2 start : 시작 점
@@ Vector2 end : 끝 점
*******************************************************************************/
void Vector2::SetDirVector(Vector2 start, Vector2 end)
{
	Vector2 dir = end - start;
	(*this) = dir.Normalize();
}
/*******************************************************************************
## SetDirVector ##
현재 벡터를 지정 각도를 가르키는 방향벡터로 설정한다.
@@ float degree : 디그리 값
*******************************************************************************/
void Vector2::SetDirVector(float degree)
{
	x = cosf(Math::ToRadian(degree));
	y = -sinf(Math::ToRadian(degree));
}
/*******************************************************************************
## GetDegree ##
현재 벡터의 디그리 값을 구해 반환
*******************************************************************************/
float Vector2::GetDegree()
{
	int sign = 1;
	Vector2 dir = *this;
	dir = dir.Normalize();
	(dir.y > 0) ? sign = -1 : sign = 1;
	return sign * Math::ToDegree(acosf(dir.x));
}
/*******************************************************************************
## GetLength ##
현재 벡터의 크기를 반환
*******************************************************************************/
float Vector2::GetLength()
{
	return sqrtf(x*x + y * y);
}
/*******************************************************************************
## Fixedx ##
현재 벡터X가 min,max를 넘어 갈려고 하면 막아준다.
@@ float min : 최소값
@@ float max : 최대값
*******************************************************************************/
void Vector2::FixedX(float min, float max)
{
	if (x < min) x = min;
	if (x > max) x = max;
}
/*******************************************************************************
## FixedY ##
현재 벡터 Y가 min,max를 넘어 갈려고 하면 막아준다.
@@ float min : 최소값
@@ float max : 최대값
*******************************************************************************/
void Vector2::FixedY(float min, float max)
{
	if (y < min) y = min;
	if (y > max) y = max;
}
/*******************************************************************************
## Dot ##
내 벡터와 v벡터의 내적값을 반환
@@ Vectir2 v : 내적할 벡터
*******************************************************************************/
float Vector2::Dot(Vector2 v)
{
	return x * v.x + y * v.y;
}
/*******************************************************************************
## Normalize ##
벡터를 정규화한 값을 반환
@@ Vector2* vector : 정규화 할 벡터 포인터
*******************************************************************************/
void Vector2::Normalize(Vector2 * vector)
{
	Vector2 result;
	float size = 1;

	size = sqrtf(vector->x*vector->x + vector->y * vector->y);

	if (size != 0)
	{
		result.x = vector->x / size;
		result.y = vector->y / size;
	}
	else
	{
		result.x = 0;
		result.y = 0;
	}
	*vector = result;
}
/*******************************************************************************
## Dot ##
두 벡터의 내적 값을 반환
@@ Vector2* v1 : 벡터 포인터
@@ Vector2* v2 : 벡터 포인터
*******************************************************************************/
float Vector2::Dot(Vector2 * v1, Vector2 * v2)
{
	return v1->x * v2->x + v1->y * v2->y;
}
