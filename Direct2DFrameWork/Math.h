#pragma once
#include <math.h>

#define FLOAT_EPSILON 0.001f									//���Ƿ�(float)
#define FLOAT_TO_INT(f1) static_cast<int>(f1 + FLOAT_EPSILON)	//float�� �ùٸ� int������ ��ȯ
#define FLOAT_EQUAL(f1, f2) (fabs(f1-f2) <= FLOAT_EPSILON)		//�ε��Ҽ��� ���� ������ ������ ��ȯ
#define FLOAT_ZERO(a)		(abs(a) < FEPSLON)					//�ε��Ҽ��� ���� ������ 0���� 

class Math
{
public:
	static const float PI;					//PI 3.14f
	static const float Epsilon;				//���Ƿ�(�ε��Ҽ� ���� ����)

	static const int IntMin;				//int �ּ�
	static const int IntMax;				//int �ִ�
	static const float FloatMin;			//float �ּ�
	static const float FloatMax;			//float �ִ�

	static float NegativeChecking(float value);		//���� ��� �Ǵ�
	static float ToRadian(float degree);			//��׸� -> ����
	static float ToDegree(float radian);			//���� -> ��׸� 

	static int Random(int r1, int r2);				//int ������ 
	static float Random(float r1, float r2);		//float ������

	static float RandF();							//float ������

	static float Lerp(float val1, float val2, float amount);	//���� ������
	static float Clampf(float value, float min, float max);		//value�� min,max������ �Ѿ���ϸ� min,max�� ��ȯ
	static int Clamp(int value, int min, int max);				//''

	static float Modulo(float val1, float val2);

	static bool EqualFloat(float f1, float f2);


	/*********************************************************
	## Min ##
	�� �� �߿��� ���� ���� ��ȯ
	@@ T a : ��
	@@ T b : ��
	**********************************************************/
	template<typename T>
	static T Min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}
	/**********************************************************
	## Max ##
	�� �� �߿��� ū ���� ��ȯ
	@@ T a : ��
	@@ T b : ��
	**********************************************************/
	template<typename T>
	static T Max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}
	/**********************************************************
	## Lerp ##
	a��b�� ���� ���� ���� ���� ��ȯ
	@@ T a : ��
	@@ T b : ��
	@@ float t : 0.0f ~ 1.0f ������ ��(a �� b ������ ���߰�(����))
	**********************************************************/
	template<typename T>
	static T Lerp(const T& a, const T& b, float t)
	{
		return a + (b - a)*t;
	}
	/**********************************************************
	## Abs ##
	���밪 ��ȯ
	@@ T a : ��
	**********************************************************/
	template<typename T>
	static T Abs(const T& a)
	{
		if (a >= 0.0f)
			return a;
		else
			return -a;
	}
	/**********************************************************
	## IsInRange ##
	@@ a : ù��° �񱳴��
	@@ b : �ι�° �񱳴��
	@@ range : ����
	**********************************************************/
	template<typename T>
	static bool IsInRange(const T& a, const T& b, const T& range)
	{
		if (Abs(a - b) <= range)
			return true;
		else
			return false;
	}
	//�� ���� �Ÿ��ȿ� ���Դ��� Ȯ��
	static bool InDistance(Vector2 start, Vector2 end, float maxDistance);
	//�� �������� �Ÿ� ��ȯ
	static float GetDistance(float startX, float startY, float endX, float endY);
	//�� �� ������ ���� ��ȯ
	static float GetAngle(float x1, float y1, float x2, float y2);
};