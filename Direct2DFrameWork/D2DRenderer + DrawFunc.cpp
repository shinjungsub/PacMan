#include "stdafx.h"
#include "D2DRenderer.h"

/**********************************************************************************************
## RenderText ##
@@ int x : x ��ǥ
@@ int y : y ��ǥ
@@ wstring text : �׸� �ؽ�Ʈ
@@ int size : ��Ʈ ������
@@ DefaultBrush brush : �׸� �귯�� = DefaultBrush::Black
@@ DWRITE_TEXT_ALIGNMENT align : ���� �ɼ� = DWRITE_TEXT_ALIGNMENT_LEADING
@@ bool isRelative : ī�޶� ���� ���� = false
@@ wstring font : ��Ʈ = �������

�⺻ �����ص� �귯���� �ؽ�Ʈ ������
************************************************************************************************/
void D2DRenderer::RenderText( int  x,  int  y,  wstring  text,  int  size,
	 DefaultBrush defaultBrush,  DWRITE_TEXT_ALIGNMENT  align, wstring  font)
{
	POINTFLOAT pos = { x, y };
	IDWriteTextLayout* layout = nullptr;
	HRESULT hr = this->mDWFactory->CreateTextLayout
	(
		text.c_str(),
		text.length(),
		this->mFontList[font],
		(float)text.length() * size,
		(float)size,
		&layout
	);
	assert(SUCCEEDED(hr));

	DWRITE_TEXT_RANGE range;
	range.startPosition = 0;
	range.length = text.length();

	layout->SetFontSize((float)size, range);
	layout->SetTextAlignment(align);

	mD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	mD2DRenderTarget->DrawTextLayout(D2D1::Point2F(pos.x, pos.y), layout,
		mDefaultBrushList[(UINT)defaultBrush]);

	SAFE_RELEASE(layout);
}
/**********************************************************************************************
## RenderText ##
@@ int x : �׸���ǥ
@@ int y : �׸���ǥ
@@ wstring text : �׸� �ؽ�Ʈ
@@ COLORREF color : RGB�÷�
@@ float alpha : ���� ��
@@ int size : ��Ʈ ������
@@ DWRITE_TEXT_ALIGNMENT align : ���Ŀɼ�
@@ wstring font : ��Ʈ

���ο� �귯�� ������ ������
************************************************************************************************/
void D2DRenderer::RenderText( int  x,  int  y,  wstring  text,  COLORREF  color,
	 float  alpha,  int  size,  DWRITE_TEXT_ALIGNMENT  align,  wstring  font)
{
	POINTFLOAT pos = {x, y};

	IDWriteTextLayout* layout = nullptr;
	//�ؽ�Ʈ ���̾ƿ� ����
	mDWFactory->CreateTextLayout(
		text.c_str(),
		text.length(),
		mFontList[font],
		(float)text.length() * size,
		(float)size,
		&layout
	);

	//���̾ƿ� �¾�
	DWRITE_TEXT_RANGE range;
	range.startPosition = 0;
	range.length = text.length();
	layout->SetFontSize((float)size, range);
	layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	layout->SetTextAlignment(align);

	//�귯�� ����
	ID2D1SolidColorBrush* brush;
	mD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);

	mD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	mD2DRenderTarget->DrawTextLayout(D2D1::Point2F(pos.x, pos.y), layout, brush);

	SAFE_RELEASE(brush);
	SAFE_RELEASE(layout);
}
/**********************************************************************************************
## RenderTextField ##
@@ int x : �׸� ��ǥ
@@ int y : �׸� ��ǥ
@@ wstring text : �׸� �ؽ�Ʈ
@@ int size : ��Ʈ ������
@@ int width : ���� ���α���
@@ int heifht : ���� ���α���
@@ DefaultBrush brush : �⺻ ���� �귯��
@@ DWRITE_TEXT_ALIGNMENT align : ���� �ɼ�
@@ wstring font : ��Ʈ

���������� �⺻ �귯���� ���� ���
************************************************************************************************/
void D2DRenderer::RenderTextField( int  x,  int  y,  wstring  text,  int  size,
	 int  width,  int  height,  DefaultBrush  defaultBrush,  DWRITE_TEXT_ALIGNMENT  align,  wstring  font)
{
	POINTFLOAT pos = { x, y };

	IDWriteTextLayout* layout = nullptr;
	mDWFactory->CreateTextLayout(
		text.c_str(),
		text.length(),
		mFontList[font],
		(float)width,
		(float)height,
		&layout
	);

	//���̾ƿ� �¾�
	DWRITE_TEXT_RANGE range;
	range.startPosition = 0;
	range.length = text.length();
	layout->SetFontSize((float)size, range);

	layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	layout->SetTextAlignment(align);

	mD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	mD2DRenderTarget->DrawTextLayout(D2D1::Point2F(pos.x, pos.y), layout, mDefaultBrushList[(UINT)defaultBrush]);

	SAFE_RELEASE(layout);
}
/**********************************************************************************************
## RenderText ##
@@ int x : �׸� ��ǥ
@@ int y : �׸� ��ǥ
@@ wstring text : �׸� �ؽ�Ʈ
@@ COLORREF color : �׸� �÷�
@@ int size : ��Ʈ ������
@@ int width : ���� ���� ����
@@ int height : ���� ���� ����
@@ float alpha : ���İ�
@@ DWRITE_TEXT_ALIGNMENT align : ���� �ɼ�
@@ wstring font : ��Ʈ

�����ȿ��� ���ο� �귯���� �����ؼ� ��Ʈ���
************************************************************************************************/
void D2DRenderer::RenderTextField( int  x,  int  y,  wstring  text,  COLORREF  color,
	 int  size,  int  width,  int  height,  float  alpha,  DWRITE_TEXT_ALIGNMENT  align,  wstring  font)
{
	POINTFLOAT pos = { x, y };
	IDWriteTextLayout* layout = nullptr;
	mDWFactory->CreateTextLayout(
		text.c_str(),
		text.length(),
		mFontList[font],
		(float)width,
		(float)height,
		&layout
	);

	//���̾ƿ� �¾�
	DWRITE_TEXT_RANGE range;
	range.startPosition = 0;
	range.length = text.length();
	layout->SetFontSize((float)size, range);

	layout->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	layout->SetTextAlignment(align);

	//�귯�� ����
	ID2D1SolidColorBrush* brush;
	mD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);

	mD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	mD2DRenderTarget->DrawTextLayout(D2D1::Point2F((float)x, (float)y), layout, brush);

	SAFE_RELEASE(layout);
	SAFE_RELEASE(brush);
}
/**********************************************************************************************
## DrawLine ##
@@ POINTFLOAT start : ���� ��
@@ POINTFLOAT end : �� ��
@@ D2D1::ColorF::Enum color : ����
@@ float alpha : ���� ��
@@ float strokeWidth : �� ����
************************************************************************************************/
void D2DRenderer::DrawLine( POINTFLOAT start,  POINTFLOAT  end,  D2D1::ColorF::Enum  color,
	 float  alpha,  float  strokeWidth)
{
	ID2D1SolidColorBrush* brush;
	mD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);

	mD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());


	mD2DRenderTarget->DrawLine(D2D1::Point2F(start.x, start.y), D2D1::Point2F(end.x, end.y), brush, strokeWidth);
	SAFE_RELEASE(brush);
}
/**********************************************************************************************
## DrawLine ##
@@ POINTFLOAT start : ���� ��
@@ POINTFLOAT end : �� ��
@@ DefaultBrush brush : �⺻ ���� �귯��
@@ float stroke : �� ����
***********************************************************************************************/
void D2DRenderer::DrawLine( POINTFLOAT  start,  POINTFLOAT  end,  DefaultBrush  defaultBrush,float  strokeWidth)
{
	mD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	mD2DRenderTarget->DrawLine(D2D1::Point2F(start.x, start.y),
		D2D1::Point2F(end.x, end.y), mDefaultBrushList[(UINT)defaultBrush], strokeWidth);
}
/**********************************************************************************************
## AlphaDrawLine ##
@@ POINTFLOAT start : ���� ��
@@ POINTFLOAT end : �� ��
@@ DefaultBrush brush : �⺻ ���� �귯��
@@ float stroke : �� ����
***********************************************************************************************/
void D2DRenderer::AlphaDrawLine(POINTFLOAT  start, POINTFLOAT  end, DefaultBrush  defaultBrush, float alpha, float  strokeWidth)
{
	mD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	mD2DRenderTarget->DrawLine(D2D1::Point2F(start.x, start.y),
		D2D1::Point2F(end.x, end.y), mDefaultBrushList[(UINT)defaultBrush], strokeWidth);
	mDefaultBrushList[(UINT)defaultBrush]->SetOpacity(alpha);
}
/**********************************************************************************************
## DrawRectangle ##
@@ RECT rc : ��Ʈ
@@ D2D1::ColorF::Enum color : �÷�
@@ float alpha : ���� ��
@@ float strokeWidth : �� ����
************************************************************************************************/
void D2DRenderer::DrawRectangle( RECT  rc,  D2D1::ColorF::Enum  color,
	 float  alpha,   float  strokeWidth)
{
	ID2D1SolidColorBrush* brush;
	mD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);

	mD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	mD2DRenderTarget->DrawRectangle(D2D1::RectF((float)rc.left, (float)rc.top, (float)rc.right, (float)rc.bottom),
		brush, strokeWidth);

	SAFE_RELEASE(brush);
}
/**********************************************************************************************
## DrawRectangle ##
@@ RECT rc : �׸� ��Ʈ
@@ DefaultBrush brush : �׸� �귯��
@@ float stroke : �� ����
************************************************************************************************/
void D2DRenderer::DrawRectangle( RECT  rc,  DefaultBrush  defaultBrush, float  strokeWidth)
{
	mD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	mD2DRenderTarget->DrawRectangle(D2D1::RectF((float)rc.left, (float)rc.top, (float)rc.right, (float)rc.bottom),
		mDefaultBrushList[(UINT)defaultBrush], strokeWidth);

}
/**********************************************************************************************
## DrawEllipse ##
@@ POINTFLOAT origin : ����
@@ flaot radius : ������
@@ DefaultBrush brush : �귯��
@@ float stroke : �� ����
************************************************************************************************/
void D2DRenderer::DrawEllipse(POINTFLOAT origin, float radius, DefaultBrush defaultBrush, float strokeWidth)
{
	RECT rc = Figure::RectMakePivot(origin.x, origin.y, radius, radius, Pivot::CENTER);
	POINTFLOAT pos = origin;

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = pos.x;
	ellipse.point.y = pos.y;
	ellipse.radiusX = radius;
	ellipse.radiusY = radius;

	mD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	mD2DRenderTarget->DrawEllipse(&ellipse, mDefaultBrushList[(UINT)defaultBrush], strokeWidth);
}
/**********************************************************************************************
## DrawEllipse ##
@@ POINTFLOAT origin : ����
@@ flaot radius : ������
@@ D2D1::ColorF::Enum color : D2D�÷� ��
@@ float alpha : ���� ��
@@ float stroke : �� ����
************************************************************************************************/
void D2DRenderer::DrawEllipse( POINTFLOAT origin,  float  radius,  D2D1::ColorF::Enum  color,
	 float  alpha,   float  strokeWidth)
{
	RECT rc = Figure::RectMakePivot(origin.x, origin.y, radius, radius, Pivot::CENTER);
	POINTFLOAT pos = origin;

	ID2D1SolidColorBrush* brush(nullptr);
	mD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = pos.x;
	ellipse.point.y = pos.y;
	ellipse.radiusX = radius;
	ellipse.radiusY = radius;

	mD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	mD2DRenderTarget->DrawEllipse(&ellipse, brush, strokeWidth);

	SAFE_RELEASE(brush);
}
/**********************************************************************************************
## FillRectagle ##
@@ RECT rc : �׸� ��Ʈ
@@ D2D1::ColorF::Enum color : D2D�÷�
@@ float alpha  : ���� ��
************************************************************************************************/
void D2DRenderer::FillRectangle( RECT rc,  D2D1::ColorF::Enum  color,  float  alpha)
{
	ID2D1SolidColorBrush* brush;
	mD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);
	mD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	mD2DRenderTarget->FillRectangle(D2D1::RectF((float)rc.left, (float)rc.top, (float)rc.right, (float)rc.bottom), brush);

	SAFE_RELEASE(brush);
}
/**********************************************************************************************
## FillRectangle  ##
@@ Figure::FloatRec rc : ũ�� ��Ʈ
@@ DefaultBrush brush : �귯��
************************************************************************************************/
void D2DRenderer::FillRectangle( RECT  rc,  DefaultBrush  defaultBrush)
{
	mD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	mD2DRenderTarget->FillRectangle(D2D1::RectF((float)rc.left, (float)rc.top, (float)rc.right, (float)rc.bottom),
		mDefaultBrushList[(UINT)defaultBrush]);
}
/**********************************************************************************************
## FillEllipse ##
@@ POINTFLOAT origin : ���� ��ǥ
@@ flaot radius : ������
@@ D2D1::ColorF::Enum color : D2D�÷�
@@ float alpha : ���� ��
************************************************************************************************/
void D2DRenderer::FiilEllipse( POINTFLOAT  origin,  float  radius,  D2D1::ColorF::Enum  color, float  alpha)
{
	RECT rc = Figure::RectMakePivot(origin.x, origin.y, radius, radius, Pivot::CENTER);
	POINTFLOAT pos = origin;

	ID2D1SolidColorBrush* brush(nullptr);
	mD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color, alpha), &brush);

	D2D1_ELLIPSE ellipse;
	ellipse.point.x = origin.x;
	ellipse.point.y = origin.y;
	ellipse.radiusX = radius;
	ellipse.radiusY = radius;

	mD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	mD2DRenderTarget->FillEllipse(&ellipse, brush);
	SAFE_RELEASE(brush);
}
/**********************************************************************************************
## FillEllipse ##
@@ POINTFLOAT origin : ���� ��ǥ
@@ float radius : ������
@@ DefaultBrush brush : �귯��
@@ bool isRlative : ī�޶� ���� ����
************************************************************************************************/
void D2DRenderer::FiilEllipse( POINTFLOAT origin,  float radius,  DefaultBrush  brush)
{
	RECT rc = Figure::RectMakePivot(origin.x, origin.y, radius, radius, Pivot::CENTER);
	POINTFLOAT pos = origin;
	
	D2D1_ELLIPSE ellipse;
	ellipse.point.x = origin.x;
	ellipse.point.y = origin.y;
	ellipse.radiusX = radius;
	ellipse.radiusY = radius;

	mD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	mD2DRenderTarget->FillEllipse(&ellipse, mDefaultBrushList[(UINT)brush]);
}
