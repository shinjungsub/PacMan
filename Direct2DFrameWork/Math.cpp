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
�Ҽ��� ���� ����� �Ǵ��� ~1.f�Ǵ� 1.f ��ȯ
@@ float value : ��
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
��׸����� �������� ��ȯ�� ��ȯ
@@ float degree : ��׸���
**********************************************************/
float Math::ToRadian(float degree)
{
	return degree * Math::PI / 180.0f;
}
/**********************************************************
## ToDegree ##
������ ��׸��� ��ȯ�� ��ȯ
@@ float radian : ���� ��
**********************************************************/
float Math::ToDegree(float radian)
{
	return radian * 180.0f / Math::PI;
}
/**********************************************************
## Random ##
@@ float r1 : ���� ��
@@ float r2 : �� ��
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
�Ҽ��� ���� ��ȯ
**********************************************************/
float Math::RandF()
{
	return (float)(rand()) / (float)RAND_MAX;
}
/**********************************************************
## Lerp ##
������ ���� �� ���� ���� �����ؼ� ��ȯ
@@ float val1 : ���� ��
@@ float val2 : ���� ��
@@ float amount : �� ���� ����, 0.0f ~ 1.0f ������ ���� ����.
**********************************************************/
float Math::Lerp(float val1, float val2, float amount)
{
	return val1 + (val2 - val1) * amount;
}
/**********************************************************
## Clampf ##
��� ���� min,max���� ũ�ų� �۾������� �ϸ� min�Ǵ�max�� ��ȯ
@@ float value : ��
@@ float min : �ּ� ����
@@ float max : �ִ� ����
**********************************************************/
float Math::Clampf(float value, float min, float max)
{
	value = value >= max ? max : value;
	value = value <= min ? min : value;

	return value;
}
/**********************************************************
## Clamp ##
��� ���� min,max���� ũ�ų� �۾������� �ϸ� min�Ǵ�max�� ��ȯ
@@ int value : ��
@@ int min : �ּ� ����
@@ int max : �ִ� ����
**********************************************************/
int Math::Clamp(int value, int min, int max)
{
	value = value > max ? max : value;
	value = value < min ? min : value;

	return value;
}

/***********************************************************
## Modulo ##
��� ������ ��� �ȳ�
***********************************************************/
float Math::Modulo(float val1, float val2)
{
	while (val1 - val2 >= 0)
		val1 -= val2;

	return val1;
}
/*********************************************************************
## InDistance ##
�� �� ������ �Ÿ��� maxDistance�� �Ѿ���� Ȯ�� �뵵
sqrt �Լ��� ����� ũ�� ������ �������¿��� Ȯ��
@@ vector2 start : ������
@@ vector2 end : ��ǥ��
@@ float maxDistance : Ȯ���ϰ� ���� �Ÿ�
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
�� �� ������ �Ÿ��� ���Ѵ�
@@ float startX : ������ X
@@ float startY : ������ y
@@ float endX : ��ǥ�� x
@@ float endY : ��ǥ�� y
**********************************************************************/
float Math::GetDistance(float startX, float startY, float endX, float endY)
{
	float x = endX - startX;
	float y = endY - startY;

	return sqrt(x * x + y * y);
}
/*******************************************************************************
## GetAngle ##
�� �� ������ ���� ���Ѵ�.
@@ float x1 : ������ X
@@ float y1 : ������ Y
@@ float x2 : ��ǥ�� X
@@ float y2 : ��ǥ�� Y
*******************************************************************************/
float Math::GetAngle(float x1, float y1, float x2, float y2)
{
	//x��� y�������� ���̸� ���Ѵ�. 
	float x = x2 - x1;
	float y = y2 - y1;
	//��Ÿ��� ������ �밢���� ���� ����
	float distance = sqrt((x * x) + (y * y));
	//�ﰢ�Լ��� ������ ���� cos(angle) = x / ����  == angle = acos(x / ����);
	float angle = acos(x / distance);
	//���� ���Ϸ��� ���� �������� �ִ� ��Ȳ�̶�� 
	if (y2 > y1)
	{
		//������ 360���� -�Ѱ��� ���Ѵ�. 
		angle = Math::PI * 2.0f - angle;
		if (angle >= Math::PI * 2.0f)
			angle -= Math::PI * 2.0f;
	}

	return angle;
}

/*******************************************************************************
## Random ##
@@ int r1 : ���� ����
@@ int r2 : �� ����
*******************************************************************************/
int Math::Random(int r1, int r2)
{
	return (int)(rand() % (r2 - r1 + 1)) + r1;
}

/*******************************************************************************
## EqualFloat ##
@@ float f1 : ù��° �Ҽ�
@@ float f2 : �ι�° �Ҽ�
*******************************************************************************/
bool Math::EqualFloat(float f1, float f2)
{
	//���� ���̸� ���밪���� ���� �� tiny float point���� �۰ų� ������ ���� ������ ����
	if (fabsf(f1 - f2) <= FLT_EPSILON) return true;
	return false;
}

