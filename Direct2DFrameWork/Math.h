#pragma once
#include <math.h>

#define FLOAT_EPSILON 0.001f									//엡실론(float)
#define FLOAT_TO_INT(f1) static_cast<int>(f1 + FLOAT_EPSILON)	//float을 올바른 int형으로 변환
#define FLOAT_EQUAL(f1, f2) (fabs(f1-f2) <= FLOAT_EPSILON)		//부동소수점 오차 내에서 같은지 반환
#define FLOAT_ZERO(a)		(abs(a) < FEPSLON)					//부동소수점 오차 내에서 0인지 

class Math
{
public:
	static const float PI;					//PI 3.14f
	static const float Epsilon;				//엡실론(부동소수 오차 범위)

	static const int IntMin;				//int 최소
	static const int IntMax;				//int 최대
	static const float FloatMin;			//float 최소
	static const float FloatMax;			//float 최대

	static float NegativeChecking(float value);		//음수 양수 판단
	static float ToRadian(float degree);			//디그리 -> 라디안
	static float ToDegree(float radian);			//라디안 -> 디그리 

	static int Random(int r1, int r2);				//int 랜덤값 
	static float Random(float r1, float r2);		//float 랜덤값

	static float RandF();							//float 랜덤값

	static float Lerp(float val1, float val2, float amount);	//선형 보간법
	static float Clampf(float value, float min, float max);		//value가 min,max범위를 넘어설려하면 min,max를 반환
	static int Clamp(int value, int min, int max);				//''

	static float Modulo(float val1, float val2);

	static bool EqualFloat(float f1, float f2);


	/*********************************************************
	## Min ##
	두 수 중에서 작은 수를 반환
	@@ T a : 수
	@@ T b : 수
	**********************************************************/
	template<typename T>
	static T Min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}
	/**********************************************************
	## Max ##
	두 수 중에서 큰 수를 반환
	@@ T a : 수
	@@ T b : 수
	**********************************************************/
	template<typename T>
	static T Max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}
	/**********************************************************
	## Lerp ##
	a와b에 대한 선형 보간 값을 반환
	@@ T a : 수
	@@ T b : 수
	@@ float t : 0.0f ~ 1.0f 사이의 값(a 와 b 사이의 가중값(비율))
	**********************************************************/
	template<typename T>
	static T Lerp(const T& a, const T& b, float t)
	{
		return a + (b - a)*t;
	}
	/**********************************************************
	## Abs ##
	절대값 반환
	@@ T a : 수
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
	@@ a : 첫번째 비교대상
	@@ b : 두번째 비교대상
	@@ range : 범위
	**********************************************************/
	template<typename T>
	static bool IsInRange(const T& a, const T& b, const T& range)
	{
		if (Abs(a - b) <= range)
			return true;
		else
			return false;
	}
	//두 점이 거리안에 들어왔는지 확인
	static bool InDistance(Vector2 start, Vector2 end, float maxDistance);
	//두 점사이의 거리 반환
	static float GetDistance(float startX, float startY, float endX, float endY);
	//두 점 사이의 각을 반환
	static float GetAngle(float x1, float y1, float x2, float y2);
};