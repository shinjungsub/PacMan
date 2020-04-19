/*********************************************************
## EnumStructHeader.h ##
여러 클래스들이 공용으로 사용할 구조체 및 Enum들
**********************************************************/
#pragma once
#include "Vector2.h"

struct TagMessage
{
	string name;
	float delayTime;
	int intData;
	Vector2 vector2Data;
	void* pData;

	TagMessage()
		:name(""), delayTime(0.f), vector2Data(0, 0), intData(0), pData(nullptr) {}
	TagMessage(string name, float delayTime = 0.f, int intData = 0, Vector2 vector2Data = Vector2(), void* pData = nullptr)
		:name(name), delayTime(delayTime), intData(intData), vector2Data(vector2Data), pData(pData) {}
};

struct TagInt
{
	int x, y;
	TagInt()
		:x(0), y(0) {}
	TagInt(int x, int y)
		:x(x), y(y) {}
};

namespace Pivot
{
	enum Enum
	{
		LEFT_TOP = 0, CENTER, BOTTOM
	};

	inline string GetPivotName(Pivot::Enum pivot)
	{
		switch (pivot)
		{
		case Pivot::LEFT_TOP:
			return "LeftTop";
		case Pivot::CENTER:
			return "Center";
		case Pivot::BOTTOM:
			return "Bottom";
		default:
			return "Error";
		}
	}

	inline Pivot::Enum GetPivotEnum(string name)
	{
		if (name == "LeftTop")
			return Pivot::LEFT_TOP;
		else if (name == "Center")
			return Pivot::CENTER;
		else if (name == "Bottom")
			return Pivot::BOTTOM;
	}

}


namespace ObjectType
{
	enum Enum : int
	{
		BackGround = 0, Object, UpObject, Effect, UI, End
	};
}

namespace Direction
{
	enum Enum : int
	{
		Right = 0, Down, Left, Up
	};
}

class GameColor
{
public:
	union
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		};
		float arr[4];
	};

	GameColor()
		:r(0.f), g(0.f), b(0.f), a(0.f) {}
	GameColor(float r, float g, float b, float a)
		:r(r), g(g), b(b), a(a) {}

	COLORREF GetColorref()
	{
		return RGB((BYTE)(arr[2] * 255.f), (BYTE)(arr[1] * 255.f), (BYTE)(arr[0] * 255.f));
	}
};