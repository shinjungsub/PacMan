#include "stdafx.h"
#include "Figure.h"

/*****************************************************************************
## CheckPointInRect ##
렉트 안에 점있는지 판단 PtInRect와 같음
@@ RECT rc : 렉트
@@ POINT pt : 포인트
******************************************************************************/
bool Figure::CheckPointInRect(const RECT& rc, const POINT& pt)
{
	if ((rc.left <= pt.x && pt.x <= rc.right) && (rc.top <= pt.y && pt.y <= rc.bottom)) return true;
	return false;
}
/***********************************************************************
## CheckPointInRect ##
렉트 안에 점이 있는지 판단
@@ RECT rc : 렉트
@@ int x : 점 x좌표
@@ int y : 점 y좌표
************************************************************************/
bool Figure::CheckPointInRect(const RECT& rc, int x, int y)
{
	if ((rc.left <= x && x <= rc.right) && (rc.top <= y && y <= rc.bottom)) return true;
	return false;
}
/***********************************************************************
## CheckPointInRect ##
@@ MYRECT rc : 내가만든 렉트 구조체
@@ float x : 점 x
@@ float y : 점 y
************************************************************************/
bool Figure::CheckPointInRect(const MYRECT& rc, float x, float y)
{
	if ((rc.left <= x && x <= rc.right) && (rc.top <= y && y <= rc.bottom)) return true;
	return false;
}
/***********************************************************************
## CheckPointInCircle ##
점이 원안에 있는지 판단
@@ float cx : 원 중심 X;
@@ float cy : 원 중심 Y;
@@ float cR : 원 반지름
@@ float x : 점 X
@@ float y : 점 Y
************************************************************************/
bool Figure::CheckPointInCircle(float cX, float cY, float cR, float x, float y)
{
	float deltaX = x - cX;
	float deltaY = y - cY;

	float distSquare = deltaX * deltaX + deltaY * deltaY;

	float radiusSquare = cR * cR;

	if (radiusSquare < distSquare) return false;

	return true;
}
/***********************************************************************
## CheckPointInCircle ##
주석 달기 귀찮다 . . . 여기까지 했으면 대충 아래는 뭔지 알겠지???
************************************************************************/
bool Figure::CheckPointInCircle(const MYCIRCLE& rc, float x, float y)
{
	float deltaX = x - rc.x;
	float deltaY = y - rc.y;

	float distSquare = deltaX * deltaX + deltaY * deltaY;

	float radiusSquare = rc.r * rc.r;

	if (radiusSquare < distSquare) return false;

	return true;
}

bool Figure::IsCollision(const MYRECT& rc1, const MYRECT& rc2)
{
	if ((rc1.left <= rc2.right && rc1.right >= rc2.left) &&
		(rc1.top <= rc2.bottom && rc1.bottom >= rc2.top)) return true;

	return false;
}

bool Figure::IsCollision(const RECT& rc1, const RECT& rc2)
{
	if ((rc1.left <= rc2.right && rc1.right >= rc2.left) &&
		(rc1.top <= rc2.bottom && rc1.bottom >= rc2.top)) return true;

	return false;
}


bool Figure::IsCollision(const MYCIRCLE& cir1, const MYCIRCLE& cir2)
{
	float deltaX = cir2.x - cir1.x;
	float deltaY = cir2.y - cir1.y;

	float distSquare = deltaX * deltaX + deltaY * deltaY;

	float radius = cir1.r + cir2.r;
	float radiusSquare = radius * radius;

	if (radiusSquare < distSquare) return false;

	return true;
}

bool Figure::IsCollision(const MYCIRCLE& cir1, const RECT& rc)
{
	int centerX = FLOAT_TO_INT(cir1.x);
	int centerY = FLOAT_TO_INT(cir1.y);
	int radius = FLOAT_TO_INT(cir1.r);

	//사각형 범위 안에 원의 중심이 있다면
	if ((rc.left <= centerX && centerX <= rc.right) ||
		(rc.top <= centerY && centerY <= rc.bottom))
	{
		//사각형을 내접하는 원에 대한 사각형 영역 생성
		RECT exRect;
		exRect.left = rc.left - radius;
		exRect.right = rc.right + radius;
		exRect.top = rc.top - radius;
		exRect.bottom = rc.bottom + radius;
		//그 사각형 영역안에 원의 중심이 있다면 충돌했다는 뜻 
		if ((exRect.left <= centerX && centerX <= exRect.right) &&
			(exRect.top <= centerY && centerY <= exRect.bottom))
		{
			return true;
		}
	}
	//사각형 범위 밖에 원의 중심이 있다면
	else
	{
		//4꼭지점에 대한 원과의 충돌을 실시한다.
		if (CheckPointInCircle(cir1, (float)rc.left, (float)rc.top)) return true;
		if (CheckPointInCircle(cir1, (float)rc.right, (float)rc.top)) return true;
		if (CheckPointInCircle(cir1, (float)rc.left, (float)rc.bottom)) return true;
		if (CheckPointInCircle(cir1, (float)rc.right, (float)rc.bottom)) return true;
	}

	return false;
}

bool Figure::IsCollision(const MYCIRCLE& cir, const MYRECT& rc)
{
	int centerX = FLOAT_TO_INT(cir.x);
	int centerY = FLOAT_TO_INT(cir.y);
	int radius = FLOAT_TO_INT(cir.r);

	if ((rc.left <= centerX && centerX <= rc.right) ||
		(rc.top <= centerY && centerY <= rc.bottom))
	{
		RECT exRect;
		exRect.left = (LONG)(rc.left - radius);
		exRect.right = (LONG)(rc.right + radius);
		exRect.top = (LONG)(rc.top - radius);
		exRect.bottom = (LONG)(rc.bottom + radius);

		if ((exRect.left <= centerX && centerX <= exRect.right) &&
			(exRect.top <= centerY && centerY <= exRect.bottom))
		{
			return true;
		}
	}
	else
	{
		if (CheckPointInCircle(cir, (float)rc.left, (float)rc.top)) return true;
		if (CheckPointInCircle(cir, (float)rc.right, (float)rc.top)) return true;
		if (CheckPointInCircle(cir, (float)rc.left, (float)rc.bottom)) return true;
		if (CheckPointInCircle(cir, (float)rc.right, (float)rc.bottom)) return true;
	}

	return false;
}


bool Figure::IsCollisionReaction(RECT& rcMove, const RECT& rcHold)
{
	RECT rcInter;

	if (IntersectRect(&rcInter, &rcHold, &rcMove) == false) return false;

	int interW = rcInter.right - rcInter.left;
	int interH = rcInter.bottom - rcInter.top;

	if (interW > interH)
	{
		//위
		if (rcInter.top == rcHold.top)
		{
			OffsetRect(&rcMove, 0, -interH);
		}
		//아래
		else if (rcInter.bottom == rcHold.bottom)
		{
			OffsetRect(&rcMove, 0, interH);
		}
	}
	else
	{
		if (rcInter.left == rcHold.left)
		{
			OffsetRect(&rcMove, -interW, 0);
		}
		else if (rcInter.right == rcHold.right)
		{
			OffsetRect(&rcMove, interW, 0);
		}
	}

	return true;
}

bool Figure::IsCollisionReaction(const MYRECT& mrcHold, MYRECT& mrcMove)
{
	RECT rcHold;
	rcHold.left = FLOAT_TO_INT(mrcHold.left);
	rcHold.top = FLOAT_TO_INT(mrcHold.top);
	rcHold.right = FLOAT_TO_INT(mrcHold.right);
	rcHold.bottom = FLOAT_TO_INT(mrcHold.bottom);

	RECT rcMove;
	rcMove.left = FLOAT_TO_INT(mrcMove.left);
	rcMove.top = FLOAT_TO_INT(mrcMove.top);
	rcMove.right = FLOAT_TO_INT(mrcMove.right);
	rcMove.bottom = FLOAT_TO_INT(mrcMove.bottom);

	RECT rcInter;
	if (!IntersectRect(&rcInter, &rcHold, &rcMove)) return false;

	int interW = rcInter.right - rcInter.left;
	int interH = rcInter.bottom - rcInter.top;

	if (interW > interH)
	{
		if (rcInter.top == rcHold.top)
		{
			OffsetRect(&rcMove, 0, -interH);
		}
		else if (rcInter.bottom == rcHold.bottom)
		{
			OffsetRect(&rcMove, 0, interH);
		}
	}
	else
	{
		if (rcInter.left == rcHold.left)
		{
			OffsetRect(&rcMove, -interW, 0);
		}
		else if (rcInter.right == rcHold.right)
		{
			OffsetRect(&rcMove, interW, 0);
		}
	}

	mrcMove.left = static_cast<float>(rcMove.left);
	mrcMove.top = static_cast<float>(rcMove.top);
	mrcMove.right = static_cast<float>(rcMove.right);
	mrcMove.bottom = static_cast<float>(rcMove.bottom);


	return true;
}

bool Figure::IsCollisionReaction(const MYCIRCLE& cirHold, MYCIRCLE& cirMove)
{
	float deltaX = cirMove.x - cirHold.x;
	float deltaY = cirMove.y - cirHold.y;
	float distance = sqrtf(deltaX * deltaX + deltaY * deltaY);

	float radius = cirHold.r + cirMove.r;

	if (distance < radius)
	{
		float angle = Math::GetAngle(cirHold.x, cirHold.y, cirMove.x, cirMove.y);

		float moveDelta = radius - distance;
		float moveX = cosf(angle) * moveDelta;
		float moveY = -sinf(angle) * moveDelta;

		cirMove.Move(moveX, moveY);

		return true;
	}

	return false;
}

void Figure::NonOverIdx(POINT& target, POINT start, POINT end)
{
	if (target.x <= start.x) target.x = start.x;
	if (target.y <= start.y) target.y = start.y;
	if (target.x >= end.x) target.x = end.x;
	if (target.y >= end.y) target.y = end.y;
}

bool Figure::IsEqualPoint(const POINT& p1, const POINT& p2)
{
	if (p1.x == p2.x && p1.y == p2.y)
		return true;
	else
		return false;
}

Figure::TagLine::TagLine(Vector2 start, Vector2 end)
	:start(start), end(end) {}

Figure::TagLine::TagLine(Vector2 start, float angle, float dist)
	: start(start)
{
	end.x = cosf(angle) * dist;
	end.y = -sinf(angle) * dist;
}


bool Figure::IntersectLineToLine(Vector2 * pCollision, TagLine lineA, TagLine lineB)
{
	float uA = ((lineB.end.x - lineB.start.x)*(lineA.start.y - lineB.start.y) - (lineB.end.y - lineB.start.y)*(lineA.start.x - lineB.start.x)) / ((lineB.end.y - lineB.start.y)*(lineA.end.x - lineA.start.x) - (lineB.end.x - lineB.start.x)*(lineA.end.y - lineA.start.y));
	float uB = ((lineA.end.x - lineA.start.x)*(lineA.start.y - lineB.start.y) - (lineA.end.y - lineA.start.y)*(lineA.start.x - lineB.start.x)) / ((lineB.end.y - lineB.start.y)*(lineA.end.x - lineA.start.x) - (lineB.end.x - lineB.start.x)*(lineA.end.y - lineA.start.y));

	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {

		if (pCollision)
		{
			pCollision->x = lineA.start.y + (uA * (lineA.end.y - lineA.start.y));
			pCollision->y = lineA.start.x + (uA * (lineA.end.x - lineA.start.x));
		}
		return true;
	}
	return false;
}
bool Figure::IntersectLineToRect(Vector2 * pCollision, TagLine line, RECT rc)
{
	Vector2 leftTop(CastingFloat(rc.left), CastingFloat(rc.top));
	Vector2 leftBottom(CastingFloat(rc.left), CastingFloat(rc.bottom));
	Vector2 rightTop(CastingFloat(rc.right), CastingFloat(rc.top));
	Vector2 rightBottom(CastingFloat(rc.right), CastingFloat(rc.bottom));
	//#   1	  #
	//4		  2
	//#   3   #
	if (IntersectLineToLine(pCollision, line, TagLine(leftTop, rightTop)))
		return true;
	if (IntersectLineToLine(pCollision, line, TagLine(rightTop, rightBottom)))
		return true;
	if (IntersectLineToLine(pCollision, line, TagLine(leftBottom, rightBottom)))
		return true;
	if (IntersectLineToLine(pCollision, line, TagLine(leftTop, leftBottom)));

	return false;
}
