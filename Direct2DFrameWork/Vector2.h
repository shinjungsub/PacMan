#pragma once

//=====================================
// ## 벡터2D ##
//=====================================
class Vector2
{
private:

public:
	float x;
	float y;

	Vector2() :x(0), y(0) {}
	Vector2(int _x, int _y) : x((float)_x), y((float)_y) {}
	Vector2(float _x, float _y) : x(_x), y(_y) {}
	Vector2(POINT pt) : x((float)pt.x), y((float)pt.y) {}
	Vector2(float angle)  //디그리 각도 (라디안 ㄴㄴ)
	{
		x = 0;
		y = 0;
		this->SetDirVector(angle);
	}
	~Vector2() {}

	//연산자 오버로딩
	Vector2 operator+ (Vector2 v)
	{
		Vector2 result;
		result.x = x + v.x;
		result.y = y + v.y;
		return result;
	}
	Vector2 operator- (Vector2 v)
	{
		Vector2 result;
		result.x = x - v.x;
		result.y = y - v.y;
		return result;
	}
	Vector2 operator* (float scalar)
	{
		Vector2 result;
		result.x = x * scalar;
		result.y = y * scalar;
		return result;
	}
	Vector2 operator* (Vector2 v)
	{
		Vector2 result;
		result.x = x * v.x;
		result.y = y * v.y;
		return result;
	}
	Vector2 operator/ (float scalar)
	{
		Vector2 result;
		result.x = x / scalar;
		result.y = y / scalar;
		return result;
	}
	bool operator==(Vector2 v)
	{
		if (x == v.x)
		{
			if (y == v.y)
			{
				return true;
			}
		}
		return false;
	}
	bool operator!=(Vector2 v)
	{
		if (!(*this == v))
		{
			return true;
		}
		return false;
	}

	void operator +=(Vector2 v)
	{
		this->x += v.x;
		this->y += v.y;
	}

	void operator -=(Vector2 v)
	{
		this->x -= v.x;
		this->y -= v.y;
	}

	//현재 벡터를 정규화시킨 값을 반환한다.
	Vector2 Normalize();

	//현재 벡터를 현 위치에서 end방향을 가르키는 방향벡터로 설정한다.
	void SetDirVector(Vector2 start, Vector2 end);
	//현재 벡터를 지정 각도를 가르키는 방향벡터로 설정한다.
	void SetDirVector(float degree);
	//현재 벡터가 가르키는 방향의 각도를 반환한다.
	float GetDegree();
	//현재 벡터의 길이를 반환한다.
	float GetLength();

	POINT ToPoint()
	{
		POINT pt;
		pt.x = (LONG)x;
		pt.y = (LONG)y;
		return pt;
	}

	//벡터를 내적한 값을 반환한다.
	float Dot(Vector2 v);

	//위치 보정
	void FixedX(float min, float max);
	void FixedY(float min, float max);

	static void Normalize(Vector2* vector);
	static float Dot(Vector2* v1, Vector2* v2);
};


