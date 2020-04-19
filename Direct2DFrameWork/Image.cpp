#include "stdafx.h"
#include "Image.h"

/********************************************************************************
## Image ##
@@ ID2D1Bitmap* bitmap : ImageManager에서 생성된 비트맵
@@ TagLoadImageInfo loadinfo : 이미지 정보(키값,파일 경로)

프레임이미지가 아닌 이미지 클래스 생성
*********************************************************************************/
Image::Image(ID2D1Bitmap * const bitmap, const TagLoadedImageInfo & loadinfo)
	:mBitmap(bitmap), mLoadInfo(loadinfo), mScale(1.f), mAlpha(1.f), mAngle(0.f), mMaxFrameX(1), mMaxFrameY(1)
{
	this->mSize.x = (float)this->mBitmap->GetPixelSize().width;
	this->mSize.y = (float)this->mBitmap->GetPixelSize().height;

	FrameRect rc;
	rc.x = 0;
	rc.y = 0;
	rc.width = mSize.x;
	rc.height = mSize.y;
	this->mFrameInfo.push_back(rc);

}
/********************************************************************************
## Image ##
@@ ID2D1Bitmap* bitmap : ImageManager에서 생성된 비트맵
@@ TagLoadImageInfo loadinfo : 이미지 정보(키값,파일 경로)
@@ int maxFrameX : 가로 프레임 수
@@ int maxFrameY : 세로 프레임 수

프레임 이미지 클래스 생성
*********************************************************************************/
Image::Image(ID2D1Bitmap * bitmap, const TagLoadedImageInfo & loadinfo, const int & maxFrameX, const int & maxFrameY)
	:mBitmap(bitmap), mLoadInfo(loadinfo), mMaxFrameX(maxFrameX), mMaxFrameY(maxFrameY), mScale(1.f), mAlpha(1.f), mAngle(0.f)
{
	this->mSize.x = (float)mBitmap->GetPixelSize().width;
	this->mSize.y = (float)mBitmap->GetPixelSize().height;

	float frameX = mSize.x / (float)this->mMaxFrameX;
	float frameY = mSize.y / (float)this->mMaxFrameY;

	FrameRect rc;
	for (int j = 0; j < maxFrameY; ++j)
	{
		for (int i = 0; i < maxFrameX; ++i)
		{
			rc.x = (float)i * (frameX);
			rc.y = (float)j * (frameY);
			rc.width = frameX;
			rc.height = frameY;
			this->mFrameInfo.push_back(rc);
		}
	}
}
/********************************************************************************
## ~Image ##
비트맵 해제
*********************************************************************************/
Image::~Image()
{
	SAFE_RELEASE(mBitmap);
}
/********************************************************************************
## Render ##
@@ int x : 그릴 좌표
@@ int y : 그릴 좌표
@@ Pivot::Enum pivot : 그릴 피봇
*********************************************************************************/
void Image::Render(float x, float y,  Pivot::Enum pivot)
{
	//그릴 사이즈 = 사이즈 * 스케일
	this->mSize.x = mSize.x * mScale;
	this->mSize.y = mSize.y * mScale;
	//렌더링 좌표
	POINTFLOAT renderPos = GetPivotPosition(x, y, pivot);

	RECT renderRc = Figure::RectMakePivot(renderPos.x, renderPos.y, mSize.x, mSize.y, Pivot::LEFT_TOP);
	//크기 정보를 기반으로 scale행렬 구축 
	D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(mScale, mScale);
	POINTFLOAT offset = { 0, 0 };
	//가로로 뒤집을 것인가
	if (mIsReverseAxisX)
	{
		scale = D2D1::Matrix3x2F::Scale(-mScale, mScale);
		offset.x = mSize.x;
	}
	//세로로 뒤집을 것인가
	if (mIsReverseAxisY)
	{
		scale = D2D1::Matrix3x2F::Scale(mScale, -mScale);
		offset.y = mSize.y;
	}
	//angle정보를 기준으로 회전 행렬 구축 
	D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(mAngle, D2D1::Point2F(mSize.x / 2.f, mSize.y / 2.f));
	//위치 정보와 offset값을 기반으로 이동 행렬 구축
	D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation((renderPos.x + offset.x), (renderPos.y + offset.y));
	//렌더링 영역 생성 
	D2D1_RECT_F dxArea = D2D1::RectF(0.f, 0.f, mSize.x, mSize.y);
	//이미지가 화면에 그려질 최종 행렬은 크기행렬 * 회전행렬 * 이동행렬 
	//행렬은 교환법칙이 성립이 안되므로 반드시 위의 순서를 맞춰서 진행해 주어야 한다. 
	D2DRENDERER->GetRenderTarget()->SetTransform(scale * rotation * translation);
	//렌더링 요청
	D2DRENDERER->GetRenderTarget()->DrawBitmap(mBitmap, dxArea, mAlpha);
	//렌더링이 끝났다면 옵션값 기본으로 세팅
	this->ResetRenderOption();
}
/********************************************************************************
## StretchRender ##
@@ int x : 그릴 좌표
@@ int y : 그릴 좌표
@@ Pivot::Enum pivot : 그릴 피봇
*********************************************************************************/
void Image::StretchRender(float x, float y, float _scale, Pivot::Enum pivot)
{
	mScale = _scale;
	//그릴 사이즈 = 사이즈 * 스케일
	this->mSize.x = mSize.x * mScale;
	this->mSize.y = mSize.y * mScale;
	//렌더링 좌표
	POINTFLOAT renderPos = GetPivotPosition(x, y, pivot);

	RECT renderRc = Figure::RectMakePivot(renderPos.x, renderPos.y, mSize.x, mSize.y, Pivot::LEFT_TOP);
	//크기 정보를 기반으로 scale행렬 구축 
	D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(mScale, mScale);
	POINTFLOAT offset = { 0, 0 };
	//가로로 뒤집을 것인가
	if (mIsReverseAxisX)
	{
		scale = D2D1::Matrix3x2F::Scale(-mScale, mScale);
		offset.x = mSize.x;
	}
	//세로로 뒤집을 것인가
	if (mIsReverseAxisY)
	{
		scale = D2D1::Matrix3x2F::Scale(mScale, -mScale);
		offset.y = mSize.y;
	}
	//angle정보를 기준으로 회전 행렬 구축 
	D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(mAngle, D2D1::Point2F(mSize.x / 2.f, mSize.y / 2.f));
	//위치 정보와 offset값을 기반으로 이동 행렬 구축
	D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation((renderPos.x + offset.x), (renderPos.y + offset.y));
	//렌더링 영역 생성 
	D2D1_RECT_F dxArea = D2D1::RectF(0.f, 0.f, mSize.x, mSize.y);
	//이미지가 화면에 그려질 최종 행렬은 크기행렬 * 회전행렬 * 이동행렬 
	//행렬은 교환법칙이 성립이 안되므로 반드시 위의 순서를 맞춰서 진행해 주어야 한다. 
	D2DRENDERER->GetRenderTarget()->SetTransform(scale * rotation * translation);
	//렌더링 요청
	D2DRENDERER->GetRenderTarget()->DrawBitmap(mBitmap, dxArea, mAlpha);
	//렌더링이 끝났다면 옵션값 기본으로 세팅
	this->ResetRenderOption();
}
/********************************************************************************
## StretchRotationRender ##
@@ int x : 그릴 좌표
@@ int y : 그릴 좌표
@@ Pivot::Enum pivot : 그릴 피봇
*********************************************************************************/
void Image::StretchRotationRender(float x, float y, float _scale, float angle, Pivot::Enum pivot)
{
	mScale = _scale;
	mAngle = -angle;
	//그릴 사이즈 = 사이즈 * 스케일
	this->mSize.x = mSize.x * mScale;
	this->mSize.y = mSize.y * mScale;
	//렌더링 좌표
	POINTFLOAT renderPos = GetPivotPosition(x, y, pivot);

	RECT renderRc = Figure::RectMakePivot(renderPos.x, renderPos.y, mSize.x, mSize.y, Pivot::LEFT_TOP);
	//크기 정보를 기반으로 scale행렬 구축 
	D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(mScale, mScale);
	POINTFLOAT offset = { 0, 0 };
	//가로로 뒤집을 것인가
	if (mIsReverseAxisX)
	{
		scale = D2D1::Matrix3x2F::Scale(-mScale, mScale);
		offset.x = mSize.x;
	}
	//세로로 뒤집을 것인가
	if (mIsReverseAxisY)
	{
		scale = D2D1::Matrix3x2F::Scale(mScale, -mScale);
		offset.y = mSize.y;
	}
	//angle정보를 기준으로 회전 행렬 구축 
	D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(mAngle, D2D1::Point2F(mSize.x / 2.f, mSize.y / 2.f));
	//위치 정보와 offset값을 기반으로 이동 행렬 구축
	D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation((renderPos.x + offset.x), (renderPos.y + offset.y));
	//렌더링 영역 생성 
	D2D1_RECT_F dxArea = D2D1::RectF(0.f, 0.f, mSize.x, mSize.y);
	//이미지가 화면에 그려질 최종 행렬은 크기행렬 * 회전행렬 * 이동행렬 
	//행렬은 교환법칙이 성립이 안되므로 반드시 위의 순서를 맞춰서 진행해 주어야 한다. 
	D2DRENDERER->GetRenderTarget()->SetTransform(scale * rotation * translation);
	//렌더링 요청
	D2DRENDERER->GetRenderTarget()->DrawBitmap(mBitmap, dxArea, mAlpha);
	//렌더링이 끝났다면 옵션값 기본으로 세팅
	this->ResetRenderOption();
}
/********************************************************************************
## Render ##
@@ int x : 그릴 좌표
@@ int y : 그릴 좌표
@@ Pivot::Enum pivot : 그릴 피봇
*********************************************************************************/
void Image::rotationRender(float x, float y, float angle, Pivot::Enum pivot)
{
	//그릴 사이즈 = 사이즈 * 스케일
	this->mSize.x = mSize.x * mScale;
	this->mSize.y = mSize.y * mScale;
	//렌더링 좌표
	POINTFLOAT renderPos = GetPivotPosition(x, y, pivot);
	mAngle = -angle;

	RECT renderRc = Figure::RectMakePivot(renderPos.x, renderPos.y, mSize.x, mSize.y, Pivot::LEFT_TOP);
	//크기 정보를 기반으로 scale행렬 구축 
	D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(mScale, mScale);
	POINTFLOAT offset = { 0, 0 };
	//가로로 뒤집을 것인가
	if (mIsReverseAxisX)
	{
		scale = D2D1::Matrix3x2F::Scale(-mScale, mScale);
		offset.x = mSize.x;
	}
	//세로로 뒤집을 것인가
	if (mIsReverseAxisY)
	{
		scale = D2D1::Matrix3x2F::Scale(mScale, -mScale);
		offset.y = mSize.y;
	}
	//angle정보를 기준으로 회전 행렬 구축 
	D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(mAngle, D2D1::Point2F(mSize.x / 2.f, mSize.y / 2.f));
	//위치 정보와 offset값을 기반으로 이동 행렬 구축
	D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation((renderPos.x + offset.x), (renderPos.y + offset.y));
	//렌더링 영역 생성 
	D2D1_RECT_F dxArea = D2D1::RectF(0.f, 0.f, mSize.x, mSize.y);
	//이미지가 화면에 그려질 최종 행렬은 크기행렬 * 회전행렬 * 이동행렬 
	//행렬은 교환법칙이 성립이 안되므로 반드시 위의 순서를 맞춰서 진행해 주어야 한다. 
	D2DRENDERER->GetRenderTarget()->SetTransform(scale * rotation * translation);
	//렌더링 요청
	D2DRENDERER->GetRenderTarget()->DrawBitmap(mBitmap, dxArea, mAlpha);
	//렌더링이 끝났다면 옵션값 기본으로 세팅
	this->ResetRenderOption();
}
/********************************************************************************
## Render ##
@@ int x : 그릴 좌표
@@ int y : 그릴 좌표
@@ Pivot::Enum pivot : 그릴 피봇
*********************************************************************************/
void Image::AlphaRender(float x, float y, float alpha, Pivot::Enum pivot)
{
	//그릴 사이즈 = 사이즈 * 스케일
	this->mSize.x = mSize.x * mScale;
	this->mSize.y = mSize.y * mScale;
	//렌더링 좌표
	POINTFLOAT renderPos = GetPivotPosition(x, y, pivot);
	mAlpha = alpha;

	RECT renderRc = Figure::RectMakePivot(renderPos.x, renderPos.y, mSize.x, mSize.y, Pivot::LEFT_TOP);
	//크기 정보를 기반으로 scale행렬 구축 
	D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(mScale, mScale);
	POINTFLOAT offset = { 0, 0 };
	//가로로 뒤집을 것인가
	if (mIsReverseAxisX)
	{
		scale = D2D1::Matrix3x2F::Scale(-mScale, mScale);
		offset.x = mSize.x;
	}
	//세로로 뒤집을 것인가
	if (mIsReverseAxisY)
	{
		scale = D2D1::Matrix3x2F::Scale(mScale, -mScale);
		offset.y = mSize.y;
	}
	//angle정보를 기준으로 회전 행렬 구축 
	D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(mAngle, D2D1::Point2F(mSize.x / 2.f, mSize.y / 2.f));
	//위치 정보와 offset값을 기반으로 이동 행렬 구축
	D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation((renderPos.x + offset.x), (renderPos.y + offset.y));
	//렌더링 영역 생성 
	D2D1_RECT_F dxArea = D2D1::RectF(0.f, 0.f, mSize.x, mSize.y);
	//이미지가 화면에 그려질 최종 행렬은 크기행렬 * 회전행렬 * 이동행렬 
	//행렬은 교환법칙이 성립이 안되므로 반드시 위의 순서를 맞춰서 진행해 주어야 한다. 
	D2DRENDERER->GetRenderTarget()->SetTransform(scale * rotation * translation);
	//렌더링 요청
	D2DRENDERER->GetRenderTarget()->DrawBitmap(mBitmap, dxArea, mAlpha);
	//렌더링이 끝났다면 옵션값 기본으로 세팅
	this->ResetRenderOption();
}
/********************************************************************************
## FrameRender ##
@@ int x : 그릴 좌표
@@ int y : 그릴 좌표
@@ int frameX : 가로 프레임
@@ int frameY : 세로 프레임
@@ Pivot::Enum pivot : 그릴 피봇
*********************************************************************************/
void Image::FrameRender(float x, float y,  int frameX,  int frameY,  Pivot::Enum pivot, bool loop)
{
	//mSize = GetFrameSize();
	if (frameX < 0) frameX = 0;
	if (frameY < 0) frameY = 0;
	if (frameX >= this->mMaxFrameX) frameX = mMaxFrameX - 1;
	if (frameY >= this->mMaxFrameY) frameY = mMaxFrameY - 1;
	//프레임이 최대 프레임을 벗어났다면 그리지 않는다. 
	if (loop)
	{
		if (frameX >= this->mMaxFrameX)
			frameX = 0;
	}
	else
	{
		if (frameX >= this->mMaxFrameX || frameY >= this->mMaxFrameY)
		{
			ResetRenderOption();
			return;
		}
	}
	
	//현재 프레임인덱스 
	int frame = frameY * mMaxFrameX + frameX;
	//사이즈 = 사이즈 * 스케일 
	this->mSize.x = mSize.x * mScale;
	this->mSize.y = mSize.y * mScale;
	//그릴 좌표 
	POINTFLOAT renderPos = GetPivotPosition(x, y, pivot);

	//카메라 클리핑 
	RECT renderRc = Figure::RectMakePivot(renderPos.x, renderPos.y, mSize.x, mSize.y, Pivot::LEFT_TOP);
	
	//크기 행렬 구축 
	D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(mScale, mScale);
	POINTFLOAT offset = { 0.f, 0.f };
	if (mIsReverseAxisX)
	{
		scale = D2D1::Matrix3x2F::Scale(-mScale, mScale);
		offset.x = mSize.x;
	}
	if (mIsReverseAxisY)
	{
		scale = D2D1::Matrix3x2F::Scale(mScale, -mScale);
		offset.y = mSize.y;
	}
	//회전 행렬 구축 
	D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(mAngle, D2D1::Point2F(mSize.x / 2.f, mSize.y / 2.f));
	//이동 행렬 구축 
	D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation((renderPos.x + offset.x), (renderPos.y + offset.y));
	//그릴 영역 세팅 
	D2D1_RECT_F dxArea = D2D1::RectF(0.0f, 0.0f, mSize.x, mSize.y);
	D2D1_RECT_F dxSrc = D2D1::RectF((float)mFrameInfo[frame].x, (float)mFrameInfo[frame].y,
		(float)(mFrameInfo[frame].x + (float)mFrameInfo[frame].width),
		(float)(mFrameInfo[frame].y + (float)mFrameInfo[frame].height));
	//최종행렬 세팅
	D2DRENDERER->GetRenderTarget()->SetTransform(scale * rotation * translation);
	////렌더링 요청
	//D2DRENDERER->GetRenderTarget()->DrawBitmap(mBitmap, dxArea, mAlpha,
	//	D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &dxSrc);

	D2DRENDERER->GetRenderTarget()->DrawBitmap(mBitmap, dxArea, mAlpha,
      D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, &dxSrc);

	this->ResetRenderOption();
}
/********************************************************************************
## FrameRender ##
@@ int x : 그릴 좌표
@@ int y : 그릴 좌표
@@ int frameX : 가로 프레임
@@ int frameY : 세로 프레임
@@ Pivot::Enum pivot : 그릴 피봇
*********************************************************************************/
void Image::AlphaFrameRender(float x, float y, int frameX, int frameY, float alpha, Pivot::Enum pivot)
{
	//mSize = GetFrameSize();
	if (frameX < 0) frameX = 0;
	if (frameY < 0) frameY = 0;
	if (frameX >= this->mMaxFrameX) frameX = mMaxFrameX - 1;
	if (frameY >= this->mMaxFrameY) frameY = mMaxFrameY - 1;
	//프레임이 최대 프레임을 벗어났다면 그리지 않는다. 
	if (frameX >= this->mMaxFrameX || frameY >= this->mMaxFrameY)
	{
		ResetRenderOption();
		return;
	}
	//현재 프레임인덱스 
	int frame = frameY * mMaxFrameX + frameX;
	//사이즈 = 사이즈 * 스케일 
	this->mSize.x = mSize.x * mScale;
	this->mSize.y = mSize.y * mScale;
	//그릴 좌표 
	POINTFLOAT renderPos = GetPivotPosition(x, y, pivot);
	mAlpha = alpha;

	//카메라 클리핑 
	RECT renderRc = Figure::RectMakePivot(renderPos.x, renderPos.y, mSize.x, mSize.y, Pivot::LEFT_TOP);

	//크기 행렬 구축 
	D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(mScale, mScale);
	POINTFLOAT offset = { 0.f, 0.f };
	if (mIsReverseAxisX)
	{
		scale = D2D1::Matrix3x2F::Scale(-mScale, mScale);
		offset.x = mSize.x;
	}
	if (mIsReverseAxisY)
	{
		scale = D2D1::Matrix3x2F::Scale(mScale, -mScale);
		offset.y = mSize.y;
	}
	//회전 행렬 구축 
	D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(mAngle, D2D1::Point2F(mSize.x / 2.f, mSize.y / 2.f));
	//이동 행렬 구축 
	D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation((renderPos.x + offset.x), (renderPos.y + offset.y));
	//그릴 영역 세팅 
	D2D1_RECT_F dxArea = D2D1::RectF(0.0f, 0.0f, mSize.x, mSize.y);
	D2D1_RECT_F dxSrc = D2D1::RectF((float)mFrameInfo[frame].x, (float)mFrameInfo[frame].y,
		(float)(mFrameInfo[frame].x + mFrameInfo[frame].width),
		(float)(mFrameInfo[frame].y + mFrameInfo[frame].height));
	//최종행렬 세팅
	D2DRENDERER->GetRenderTarget()->SetTransform(scale * rotation * translation);
	//렌더링 요청
	D2DRENDERER->GetRenderTarget()->DrawBitmap(mBitmap, dxArea, mAlpha,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &dxSrc);

	this->ResetRenderOption();
}
/********************************************************************************
## ResetRenderOption ##
이미지 클래스 렌더 관련 옵션들 전부 초기화
*********************************************************************************/
void Image::ResetRenderOption()
{
	this->mAlpha = 1.0f;
	this->mScale = 1.0f;
	this->mSize.x = (float)mBitmap->GetPixelSize().width;
	this->mSize.y = (float)mBitmap->GetPixelSize().height;
	this->mIsReverseAxisX = this->mIsReverseAxisY = false;
}
/********************************************************************************
## GetPivotPosition ##
@@ int x : 그릴 좌표
@@ int y : 그릴 좌표
@@ Pivot::Enum pivot : 피봇

피봇과 크기 기반으로 좌표 반환
*********************************************************************************/
const POINTFLOAT Image::GetPivotPosition( float x,  float  y, Pivot::Enum pivot)
{
	if (pivot == Pivot::LEFT_TOP)
		return { (FLOAT)x, (FLOAT)y };

	POINTFLOAT pos = { x, y };

	switch (pivot)
	{
	case Pivot::CENTER:
		pos.x -= mSize.x * mScale * 0.5f;
		pos.y -= mSize.y * mScale * 0.5f;
		break;

	case Pivot::BOTTOM:
		pos.x -= mSize.x * mScale * 0.5f;
		pos.y -= mSize.y * mScale;
		break;
	}

	return pos;
}
