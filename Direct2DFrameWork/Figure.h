#pragma once
#define _MaxRayRange 1000.f
/*******************************************************************************
## Figure ##
도형들 관련된 함수들 편하게 쓰기 위해서 구현
*******************************************************************************/
namespace Figure
{


	struct TagLine
	{
		Vector2 start;
		Vector2 end;

		TagLine() {}
		TagLine(Vector2 start, Vector2 end);
		TagLine(Vector2 start, float angle, float dist = _MaxRayRange);
	};
	bool IntersectLineToLine(Vector2 * pCollision, TagLine lineA, TagLine lineB);
	bool IntersectLineToRect(Vector2* pCollision, TagLine line, RECT rc);

	/*******************************************************************************
	## TagCircle ##
	원 구조체
	*******************************************************************************/
	typedef struct TagCircle
	{
		float x;
		float y;
		float r;

		TagCircle() : x(0.0f), y(0.0f), r(0.0f) {}
		TagCircle(float _x, float _y, float _r) : x(_x), y(_y), r(_r) {}

		void Move(float dx, float dy)
		{
			x += dx;
			y += dy;
		}

		void Set(float centerX, float centerY, float radius)
		{
			x = centerX;
			y = centerY;
			r = radius;
		}

		void SetCenterPos(float posX, float posY)
		{
			x = posX;
			y = posY;
		}

		void Render(HDC hdc)
		{
			int x1 = FLOAT_TO_INT(x);
			int y1 = FLOAT_TO_INT(y);
			int r1 = FLOAT_TO_INT(r);

			Ellipse(hdc, x1 - r1, y1 - r1, x1 + r1, y1 + r1);
		}
	}MYCIRCLE, *LPMYCIRCLE;
	/******************************************************************************
	## TagMyRect ##
	float형 rect구조체
	*******************************************************************************/
	typedef struct TagMyRect
	{
		float left;
		float top;
		float right;
		float bottom;

		TagMyRect() : left(0.0f), top(0.0f), right(0.0f), bottom(0.0f) {}
		TagMyRect(float _left, float _top, float _right, float _bottom) :
			left(_left), top(_top), right(_right), bottom(_bottom) {}

		void Move(float dx, float dy)
		{
			left += dx;
			top += dy;
			right += dx;
			bottom += dy;
		}

		void Set(float _left, float _top, float _right, float _bottom)
		{
			left = _left;
			top = _top;
			right = _right;
			bottom = _bottom;
		}

		void SetLeftTopPos(float _left, float _top)
		{
			float width = GetWidth();
			float height = GetHeight();

			left = _left;
			top = _top;
			right = left + width;
			bottom = top + height;
		}

		void SetCenterPos(float centerX, float centerY)
		{
			float halfW = GetWidth() * 0.5f;
			float halfH = GetHeight() * 0.5f;

			left = centerX - halfW;
			top = centerY - halfH;
			right = centerX + halfW;
			bottom = centerY + halfH;
		}

		void Render(HDC hdc)
		{
			Rectangle(hdc,
				FLOAT_TO_INT(left),
				FLOAT_TO_INT(top),
				FLOAT_TO_INT(right),
				FLOAT_TO_INT(bottom));
		}
		RECT GetRect()
		{
			return { (LONG)left,(LONG)top,(LONG)right,(LONG)bottom };
		}

		float GetWidth(void) { return right - left; }
		float GetHeight(void) { return bottom - top; }

	}MYRECT, *LPMYRECT;


	bool CheckPointInRect(const RECT& rc, const POINT& pt);
	bool CheckPointInRect(const RECT& rc, int x, int y);
	bool CheckPointInRect(const MYRECT& rc, float x, float y);

	bool CheckPointInCircle(float cX, float cY, float cR, float x, float y);
	bool CheckPointInCircle(const MYCIRCLE& rc, float x, float y);

	bool IsCollision(const MYRECT& rc1, const MYRECT& rc2);
	bool IsCollision(const RECT& rc1, const RECT& rc2);

	bool IsCollision(const MYCIRCLE& cir1, const MYCIRCLE& cir2);

	bool IsCollision(const MYCIRCLE& cir1, const RECT& rc);
	bool IsCollision(const MYCIRCLE& cir, const MYRECT& rc);

	bool IsCollisionReaction(RECT& rcMove, const RECT& rcHold);
	bool IsCollisionReaction(const MYRECT& mrcHold, MYRECT& mrcMove);
	bool IsCollisionReaction(const MYCIRCLE& cirHold, MYCIRCLE& cirMove);

	void NonOverIdx(POINT& target, POINT start, POINT end);
	bool IsEqualPoint(const POINT& p1, const POINT& p2);

	/***********************************************************************
	## PointMake ##
	포인트 생성해서 반환
	************************************************************************/
	inline POINT PointMake(int x, int y)
	{
		POINT pt = { x, y };

		return pt;
	}
	/***********************************************************************
	## LineMake ##
	라인 그려줌
	@@ HDC hdc : 그릴 HDC영역
	@@ int x1 : 시작점 X;
	@@ int y1 : 시작점 Y;
	@@ int x2 : 끝점 X;
	@@ int y2 : 끝점 Y;
	************************************************************************/
	inline  void LineMake(HDC hdc, int x1, int y1, int x2, int y2)
	{
		MoveToEx(hdc, x1, y1, NULL);
		LineTo(hdc, x2, y2);
	}
	/***********************************************************************
	## RectMake ##
	좌상단 기준으로 렉트 반환
	************************************************************************/
	inline RECT RectMake(int x, int y, int width, int height)
	{
		RECT rc = { x, y, x + width, y + height };

		return rc;
	}
	/***********************************************************************
	## RectMakeCenter ##
	중심 기준으로 렉트 반환
	************************************************************************/
	inline  RECT RectMakeCenter(int x, int y, int width, int height)
	{
		RECT rc = { x - (width / 2), y - (height / 2), x + (width / 2), y + (height / 2) };

		return rc;
	}

	/***********************************************************************
	## RectMakeBottom ##
	바텀 기준으로 렉트 반환
	************************************************************************/
	inline  RECT RectMakeBottom(int x, int y, int width, int height)
	{
		RECT rc;

		rc.right = x + width / 2;
		rc.left = x - width / 2;
		rc.top = y - height;
		rc.bottom = y;

		return rc;
	}
	/***********************************************************************
	## RectMakeLeftCenter ##
	왼쪽 중심 기준으로 렉트 반환
	************************************************************************/
	inline RECT RectMakeLeftCenter(int x, int y, int width, int height)
	{
		RECT rc;
		rc.left = x;
		rc.top = y - height / 2;
		rc.right = x + width;
		rc.bottom = y + height / 2;

		return rc;
	}
	/***********************************************************************
	## MoveRect ##
	렉트를 x,y값 만큼 이동 시킨다
	@@ RECT* rc : 이동시킬 렉트 포인터
	@@ int x : x이동량
	@@ int y : y이동량
	************************************************************************/
	inline void MoveRect(RECT* rc, int x, int y)
	{
		rc->left += x;
		rc->top += y;
		rc->right += x;
		rc->bottom += y;
	}
	inline RECT RectMake(POINT pos, POINT size)
	{
		return RectMake(pos.x, pos.y, size.x, size.y);
	}
	inline RECT RectMakeCenter(POINT pos, POINT size)
	{
		return RectMakeCenter(pos.x, pos.y, size.x, size.y);
	}
	inline RECT RectMakeBottom(POINT pos, POINT size)
	{
		return RectMakeBottom(pos.x, pos.y, size.x, size.y);
	}

	inline RECT RectMakePivot(int x, int y, int width, int height, Pivot::Enum pivot)
	{
		RECT rc;
		switch (pivot)
		{
		case Pivot::LEFT_TOP:
			rc = RectMake(x, y, width, height);
			break;
		case Pivot::CENTER:
			rc = RectMakeCenter(x, y, width, height);
			break;
		case Pivot::BOTTOM:
			rc = RectMakeBottom(x, y, width, height);
			break;
		default:
			break;
		}
		return rc;
	}
}