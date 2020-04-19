#include "stdafx.h"
#include "Image.h"

/********************************************************************************
## Image ##
@@ ID2D1Bitmap* bitmap : ImageManager���� ������ ��Ʈ��
@@ TagLoadImageInfo loadinfo : �̹��� ����(Ű��,���� ���)

�������̹����� �ƴ� �̹��� Ŭ���� ����
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
@@ ID2D1Bitmap* bitmap : ImageManager���� ������ ��Ʈ��
@@ TagLoadImageInfo loadinfo : �̹��� ����(Ű��,���� ���)
@@ int maxFrameX : ���� ������ ��
@@ int maxFrameY : ���� ������ ��

������ �̹��� Ŭ���� ����
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
��Ʈ�� ����
*********************************************************************************/
Image::~Image()
{
	SAFE_RELEASE(mBitmap);
}
/********************************************************************************
## Render ##
@@ int x : �׸� ��ǥ
@@ int y : �׸� ��ǥ
@@ Pivot::Enum pivot : �׸� �Ǻ�
*********************************************************************************/
void Image::Render(float x, float y,  Pivot::Enum pivot)
{
	//�׸� ������ = ������ * ������
	this->mSize.x = mSize.x * mScale;
	this->mSize.y = mSize.y * mScale;
	//������ ��ǥ
	POINTFLOAT renderPos = GetPivotPosition(x, y, pivot);

	RECT renderRc = Figure::RectMakePivot(renderPos.x, renderPos.y, mSize.x, mSize.y, Pivot::LEFT_TOP);
	//ũ�� ������ ������� scale��� ���� 
	D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(mScale, mScale);
	POINTFLOAT offset = { 0, 0 };
	//���η� ������ ���ΰ�
	if (mIsReverseAxisX)
	{
		scale = D2D1::Matrix3x2F::Scale(-mScale, mScale);
		offset.x = mSize.x;
	}
	//���η� ������ ���ΰ�
	if (mIsReverseAxisY)
	{
		scale = D2D1::Matrix3x2F::Scale(mScale, -mScale);
		offset.y = mSize.y;
	}
	//angle������ �������� ȸ�� ��� ���� 
	D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(mAngle, D2D1::Point2F(mSize.x / 2.f, mSize.y / 2.f));
	//��ġ ������ offset���� ������� �̵� ��� ����
	D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation((renderPos.x + offset.x), (renderPos.y + offset.y));
	//������ ���� ���� 
	D2D1_RECT_F dxArea = D2D1::RectF(0.f, 0.f, mSize.x, mSize.y);
	//�̹����� ȭ�鿡 �׷��� ���� ����� ũ����� * ȸ����� * �̵���� 
	//����� ��ȯ��Ģ�� ������ �ȵǹǷ� �ݵ�� ���� ������ ���缭 ������ �־�� �Ѵ�. 
	D2DRENDERER->GetRenderTarget()->SetTransform(scale * rotation * translation);
	//������ ��û
	D2DRENDERER->GetRenderTarget()->DrawBitmap(mBitmap, dxArea, mAlpha);
	//�������� �����ٸ� �ɼǰ� �⺻���� ����
	this->ResetRenderOption();
}
/********************************************************************************
## StretchRender ##
@@ int x : �׸� ��ǥ
@@ int y : �׸� ��ǥ
@@ Pivot::Enum pivot : �׸� �Ǻ�
*********************************************************************************/
void Image::StretchRender(float x, float y, float _scale, Pivot::Enum pivot)
{
	mScale = _scale;
	//�׸� ������ = ������ * ������
	this->mSize.x = mSize.x * mScale;
	this->mSize.y = mSize.y * mScale;
	//������ ��ǥ
	POINTFLOAT renderPos = GetPivotPosition(x, y, pivot);

	RECT renderRc = Figure::RectMakePivot(renderPos.x, renderPos.y, mSize.x, mSize.y, Pivot::LEFT_TOP);
	//ũ�� ������ ������� scale��� ���� 
	D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(mScale, mScale);
	POINTFLOAT offset = { 0, 0 };
	//���η� ������ ���ΰ�
	if (mIsReverseAxisX)
	{
		scale = D2D1::Matrix3x2F::Scale(-mScale, mScale);
		offset.x = mSize.x;
	}
	//���η� ������ ���ΰ�
	if (mIsReverseAxisY)
	{
		scale = D2D1::Matrix3x2F::Scale(mScale, -mScale);
		offset.y = mSize.y;
	}
	//angle������ �������� ȸ�� ��� ���� 
	D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(mAngle, D2D1::Point2F(mSize.x / 2.f, mSize.y / 2.f));
	//��ġ ������ offset���� ������� �̵� ��� ����
	D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation((renderPos.x + offset.x), (renderPos.y + offset.y));
	//������ ���� ���� 
	D2D1_RECT_F dxArea = D2D1::RectF(0.f, 0.f, mSize.x, mSize.y);
	//�̹����� ȭ�鿡 �׷��� ���� ����� ũ����� * ȸ����� * �̵���� 
	//����� ��ȯ��Ģ�� ������ �ȵǹǷ� �ݵ�� ���� ������ ���缭 ������ �־�� �Ѵ�. 
	D2DRENDERER->GetRenderTarget()->SetTransform(scale * rotation * translation);
	//������ ��û
	D2DRENDERER->GetRenderTarget()->DrawBitmap(mBitmap, dxArea, mAlpha);
	//�������� �����ٸ� �ɼǰ� �⺻���� ����
	this->ResetRenderOption();
}
/********************************************************************************
## StretchRotationRender ##
@@ int x : �׸� ��ǥ
@@ int y : �׸� ��ǥ
@@ Pivot::Enum pivot : �׸� �Ǻ�
*********************************************************************************/
void Image::StretchRotationRender(float x, float y, float _scale, float angle, Pivot::Enum pivot)
{
	mScale = _scale;
	mAngle = -angle;
	//�׸� ������ = ������ * ������
	this->mSize.x = mSize.x * mScale;
	this->mSize.y = mSize.y * mScale;
	//������ ��ǥ
	POINTFLOAT renderPos = GetPivotPosition(x, y, pivot);

	RECT renderRc = Figure::RectMakePivot(renderPos.x, renderPos.y, mSize.x, mSize.y, Pivot::LEFT_TOP);
	//ũ�� ������ ������� scale��� ���� 
	D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(mScale, mScale);
	POINTFLOAT offset = { 0, 0 };
	//���η� ������ ���ΰ�
	if (mIsReverseAxisX)
	{
		scale = D2D1::Matrix3x2F::Scale(-mScale, mScale);
		offset.x = mSize.x;
	}
	//���η� ������ ���ΰ�
	if (mIsReverseAxisY)
	{
		scale = D2D1::Matrix3x2F::Scale(mScale, -mScale);
		offset.y = mSize.y;
	}
	//angle������ �������� ȸ�� ��� ���� 
	D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(mAngle, D2D1::Point2F(mSize.x / 2.f, mSize.y / 2.f));
	//��ġ ������ offset���� ������� �̵� ��� ����
	D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation((renderPos.x + offset.x), (renderPos.y + offset.y));
	//������ ���� ���� 
	D2D1_RECT_F dxArea = D2D1::RectF(0.f, 0.f, mSize.x, mSize.y);
	//�̹����� ȭ�鿡 �׷��� ���� ����� ũ����� * ȸ����� * �̵���� 
	//����� ��ȯ��Ģ�� ������ �ȵǹǷ� �ݵ�� ���� ������ ���缭 ������ �־�� �Ѵ�. 
	D2DRENDERER->GetRenderTarget()->SetTransform(scale * rotation * translation);
	//������ ��û
	D2DRENDERER->GetRenderTarget()->DrawBitmap(mBitmap, dxArea, mAlpha);
	//�������� �����ٸ� �ɼǰ� �⺻���� ����
	this->ResetRenderOption();
}
/********************************************************************************
## Render ##
@@ int x : �׸� ��ǥ
@@ int y : �׸� ��ǥ
@@ Pivot::Enum pivot : �׸� �Ǻ�
*********************************************************************************/
void Image::rotationRender(float x, float y, float angle, Pivot::Enum pivot)
{
	//�׸� ������ = ������ * ������
	this->mSize.x = mSize.x * mScale;
	this->mSize.y = mSize.y * mScale;
	//������ ��ǥ
	POINTFLOAT renderPos = GetPivotPosition(x, y, pivot);
	mAngle = -angle;

	RECT renderRc = Figure::RectMakePivot(renderPos.x, renderPos.y, mSize.x, mSize.y, Pivot::LEFT_TOP);
	//ũ�� ������ ������� scale��� ���� 
	D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(mScale, mScale);
	POINTFLOAT offset = { 0, 0 };
	//���η� ������ ���ΰ�
	if (mIsReverseAxisX)
	{
		scale = D2D1::Matrix3x2F::Scale(-mScale, mScale);
		offset.x = mSize.x;
	}
	//���η� ������ ���ΰ�
	if (mIsReverseAxisY)
	{
		scale = D2D1::Matrix3x2F::Scale(mScale, -mScale);
		offset.y = mSize.y;
	}
	//angle������ �������� ȸ�� ��� ���� 
	D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(mAngle, D2D1::Point2F(mSize.x / 2.f, mSize.y / 2.f));
	//��ġ ������ offset���� ������� �̵� ��� ����
	D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation((renderPos.x + offset.x), (renderPos.y + offset.y));
	//������ ���� ���� 
	D2D1_RECT_F dxArea = D2D1::RectF(0.f, 0.f, mSize.x, mSize.y);
	//�̹����� ȭ�鿡 �׷��� ���� ����� ũ����� * ȸ����� * �̵���� 
	//����� ��ȯ��Ģ�� ������ �ȵǹǷ� �ݵ�� ���� ������ ���缭 ������ �־�� �Ѵ�. 
	D2DRENDERER->GetRenderTarget()->SetTransform(scale * rotation * translation);
	//������ ��û
	D2DRENDERER->GetRenderTarget()->DrawBitmap(mBitmap, dxArea, mAlpha);
	//�������� �����ٸ� �ɼǰ� �⺻���� ����
	this->ResetRenderOption();
}
/********************************************************************************
## Render ##
@@ int x : �׸� ��ǥ
@@ int y : �׸� ��ǥ
@@ Pivot::Enum pivot : �׸� �Ǻ�
*********************************************************************************/
void Image::AlphaRender(float x, float y, float alpha, Pivot::Enum pivot)
{
	//�׸� ������ = ������ * ������
	this->mSize.x = mSize.x * mScale;
	this->mSize.y = mSize.y * mScale;
	//������ ��ǥ
	POINTFLOAT renderPos = GetPivotPosition(x, y, pivot);
	mAlpha = alpha;

	RECT renderRc = Figure::RectMakePivot(renderPos.x, renderPos.y, mSize.x, mSize.y, Pivot::LEFT_TOP);
	//ũ�� ������ ������� scale��� ���� 
	D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(mScale, mScale);
	POINTFLOAT offset = { 0, 0 };
	//���η� ������ ���ΰ�
	if (mIsReverseAxisX)
	{
		scale = D2D1::Matrix3x2F::Scale(-mScale, mScale);
		offset.x = mSize.x;
	}
	//���η� ������ ���ΰ�
	if (mIsReverseAxisY)
	{
		scale = D2D1::Matrix3x2F::Scale(mScale, -mScale);
		offset.y = mSize.y;
	}
	//angle������ �������� ȸ�� ��� ���� 
	D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(mAngle, D2D1::Point2F(mSize.x / 2.f, mSize.y / 2.f));
	//��ġ ������ offset���� ������� �̵� ��� ����
	D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation((renderPos.x + offset.x), (renderPos.y + offset.y));
	//������ ���� ���� 
	D2D1_RECT_F dxArea = D2D1::RectF(0.f, 0.f, mSize.x, mSize.y);
	//�̹����� ȭ�鿡 �׷��� ���� ����� ũ����� * ȸ����� * �̵���� 
	//����� ��ȯ��Ģ�� ������ �ȵǹǷ� �ݵ�� ���� ������ ���缭 ������ �־�� �Ѵ�. 
	D2DRENDERER->GetRenderTarget()->SetTransform(scale * rotation * translation);
	//������ ��û
	D2DRENDERER->GetRenderTarget()->DrawBitmap(mBitmap, dxArea, mAlpha);
	//�������� �����ٸ� �ɼǰ� �⺻���� ����
	this->ResetRenderOption();
}
/********************************************************************************
## FrameRender ##
@@ int x : �׸� ��ǥ
@@ int y : �׸� ��ǥ
@@ int frameX : ���� ������
@@ int frameY : ���� ������
@@ Pivot::Enum pivot : �׸� �Ǻ�
*********************************************************************************/
void Image::FrameRender(float x, float y,  int frameX,  int frameY,  Pivot::Enum pivot, bool loop)
{
	//mSize = GetFrameSize();
	if (frameX < 0) frameX = 0;
	if (frameY < 0) frameY = 0;
	if (frameX >= this->mMaxFrameX) frameX = mMaxFrameX - 1;
	if (frameY >= this->mMaxFrameY) frameY = mMaxFrameY - 1;
	//�������� �ִ� �������� ����ٸ� �׸��� �ʴ´�. 
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
	
	//���� �������ε��� 
	int frame = frameY * mMaxFrameX + frameX;
	//������ = ������ * ������ 
	this->mSize.x = mSize.x * mScale;
	this->mSize.y = mSize.y * mScale;
	//�׸� ��ǥ 
	POINTFLOAT renderPos = GetPivotPosition(x, y, pivot);

	//ī�޶� Ŭ���� 
	RECT renderRc = Figure::RectMakePivot(renderPos.x, renderPos.y, mSize.x, mSize.y, Pivot::LEFT_TOP);
	
	//ũ�� ��� ���� 
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
	//ȸ�� ��� ���� 
	D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(mAngle, D2D1::Point2F(mSize.x / 2.f, mSize.y / 2.f));
	//�̵� ��� ���� 
	D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation((renderPos.x + offset.x), (renderPos.y + offset.y));
	//�׸� ���� ���� 
	D2D1_RECT_F dxArea = D2D1::RectF(0.0f, 0.0f, mSize.x, mSize.y);
	D2D1_RECT_F dxSrc = D2D1::RectF((float)mFrameInfo[frame].x, (float)mFrameInfo[frame].y,
		(float)(mFrameInfo[frame].x + (float)mFrameInfo[frame].width),
		(float)(mFrameInfo[frame].y + (float)mFrameInfo[frame].height));
	//������� ����
	D2DRENDERER->GetRenderTarget()->SetTransform(scale * rotation * translation);
	////������ ��û
	//D2DRENDERER->GetRenderTarget()->DrawBitmap(mBitmap, dxArea, mAlpha,
	//	D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &dxSrc);

	D2DRENDERER->GetRenderTarget()->DrawBitmap(mBitmap, dxArea, mAlpha,
      D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, &dxSrc);

	this->ResetRenderOption();
}
/********************************************************************************
## FrameRender ##
@@ int x : �׸� ��ǥ
@@ int y : �׸� ��ǥ
@@ int frameX : ���� ������
@@ int frameY : ���� ������
@@ Pivot::Enum pivot : �׸� �Ǻ�
*********************************************************************************/
void Image::AlphaFrameRender(float x, float y, int frameX, int frameY, float alpha, Pivot::Enum pivot)
{
	//mSize = GetFrameSize();
	if (frameX < 0) frameX = 0;
	if (frameY < 0) frameY = 0;
	if (frameX >= this->mMaxFrameX) frameX = mMaxFrameX - 1;
	if (frameY >= this->mMaxFrameY) frameY = mMaxFrameY - 1;
	//�������� �ִ� �������� ����ٸ� �׸��� �ʴ´�. 
	if (frameX >= this->mMaxFrameX || frameY >= this->mMaxFrameY)
	{
		ResetRenderOption();
		return;
	}
	//���� �������ε��� 
	int frame = frameY * mMaxFrameX + frameX;
	//������ = ������ * ������ 
	this->mSize.x = mSize.x * mScale;
	this->mSize.y = mSize.y * mScale;
	//�׸� ��ǥ 
	POINTFLOAT renderPos = GetPivotPosition(x, y, pivot);
	mAlpha = alpha;

	//ī�޶� Ŭ���� 
	RECT renderRc = Figure::RectMakePivot(renderPos.x, renderPos.y, mSize.x, mSize.y, Pivot::LEFT_TOP);

	//ũ�� ��� ���� 
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
	//ȸ�� ��� ���� 
	D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(mAngle, D2D1::Point2F(mSize.x / 2.f, mSize.y / 2.f));
	//�̵� ��� ���� 
	D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation((renderPos.x + offset.x), (renderPos.y + offset.y));
	//�׸� ���� ���� 
	D2D1_RECT_F dxArea = D2D1::RectF(0.0f, 0.0f, mSize.x, mSize.y);
	D2D1_RECT_F dxSrc = D2D1::RectF((float)mFrameInfo[frame].x, (float)mFrameInfo[frame].y,
		(float)(mFrameInfo[frame].x + mFrameInfo[frame].width),
		(float)(mFrameInfo[frame].y + mFrameInfo[frame].height));
	//������� ����
	D2DRENDERER->GetRenderTarget()->SetTransform(scale * rotation * translation);
	//������ ��û
	D2DRENDERER->GetRenderTarget()->DrawBitmap(mBitmap, dxArea, mAlpha,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &dxSrc);

	this->ResetRenderOption();
}
/********************************************************************************
## ResetRenderOption ##
�̹��� Ŭ���� ���� ���� �ɼǵ� ���� �ʱ�ȭ
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
@@ int x : �׸� ��ǥ
@@ int y : �׸� ��ǥ
@@ Pivot::Enum pivot : �Ǻ�

�Ǻ��� ũ�� ������� ��ǥ ��ȯ
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
