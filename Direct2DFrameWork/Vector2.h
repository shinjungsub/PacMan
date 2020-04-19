#pragma once

//=====================================
// ## ����2D ##
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
	Vector2(float angle)  //��׸� ���� (���� ����)
	{
		x = 0;
		y = 0;
		this->SetDirVector(angle);
	}
	~Vector2() {}

	//������ �����ε�
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

	//���� ���͸� ����ȭ��Ų ���� ��ȯ�Ѵ�.
	Vector2 Normalize();

	//���� ���͸� �� ��ġ���� end������ ����Ű�� ���⺤�ͷ� �����Ѵ�.
	void SetDirVector(Vector2 start, Vector2 end);
	//���� ���͸� ���� ������ ����Ű�� ���⺤�ͷ� �����Ѵ�.
	void SetDirVector(float degree);
	//���� ���Ͱ� ����Ű�� ������ ������ ��ȯ�Ѵ�.
	float GetDegree();
	//���� ������ ���̸� ��ȯ�Ѵ�.
	float GetLength();

	POINT ToPoint()
	{
		POINT pt;
		pt.x = (LONG)x;
		pt.y = (LONG)y;
		return pt;
	}

	//���͸� ������ ���� ��ȯ�Ѵ�.
	float Dot(Vector2 v);

	//��ġ ����
	void FixedX(float min, float max);
	void FixedY(float min, float max);

	static void Normalize(Vector2* vector);
	static float Dot(Vector2* v1, Vector2* v2);
};


