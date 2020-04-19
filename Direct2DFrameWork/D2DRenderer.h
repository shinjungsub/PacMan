#pragma once
#include "SingletonBase.h"


class D2DRenderer final : public SingletonBase<D2DRenderer>
{
public:
	enum class DefaultBrush : UINT
	{
		White = 0, Black, Yellow, Red, Blue, Green, Gray, End
	};
private:
	typedef unordered_map<wstring, IDWriteTextFormat*>::iterator TextIter;
private:
	ID2D1Factory* mD2DFactory;
	ID2D1HwndRenderTarget* mD2DRenderTarget;

	IDWriteFactory* mDWFactory;

	ID2D1SolidColorBrush* mDefaultBrushList[((UINT)DefaultBrush::End)];
	unordered_map<wstring, IDWriteTextFormat*> mFontList;
public:
	D2DRenderer();
	virtual ~D2DRenderer();
public:
	void BeginRender();
	void EndRender();
private:
	void CreateFactory();
	void CreateRenderTarget();
	void CreateDefaultBrush();
public:
	void AddTextFormat( wstring font,  float defaultSize = 1.f);
	inline ID2D1HwndRenderTarget*const GetRenderTarget()const { return this->mD2DRenderTarget; }
public:
	//텍스트 출력(기본 브러쉬 사용) 
	void RenderText( int x,  int y,  wstring text,  int size,
		 DefaultBrush defaultBrush = DefaultBrush::Black,
		 DWRITE_TEXT_ALIGNMENT align = DWRITE_TEXT_ALIGNMENT_LEADING, 
		 wstring font = L"맑은고딕");
	//텍스트 출력 (원하는 색상과 알파값으로 브러쉬 생성해서 출력)
	void RenderText( int x,  int y,  wstring text,  COLORREF color,  float alpha,  int size,
		 DWRITE_TEXT_ALIGNMENT align = DWRITE_TEXT_ALIGNMENT_LEADING,  
		 wstring font = L"맑은고딕");
	//텍스트 필드 출력 (기본 브러쉬 사용) 
	void RenderTextField( int x,  int y,  wstring text,  int size,  int width,  int height,
		 DefaultBrush defaultBrush = DefaultBrush::Black,
		 DWRITE_TEXT_ALIGNMENT align = DWRITE_TEXT_ALIGNMENT_LEADING, 
		 wstring font = L"맑은고딕");
	//텍스트 필드 출려(원하는 색상과 알파값으로 브러쉬 생성해서 출력)
	void RenderTextField( int x,  int y,  wstring text,  COLORREF color,
		 int size,  int width,  int height,
		 float alpha = 1.0f,
		 DWRITE_TEXT_ALIGNMENT align = DWRITE_TEXT_ALIGNMENT_LEADING,  
		 wstring font = L"맑은고딕");

	void DrawLine( POINTFLOAT start, POINTFLOAT end,  D2D1::ColorF::Enum color,  float alpha,
		  float strokeWidth = 1.f);
	void DrawLine( POINTFLOAT start,  POINTFLOAT end,  DefaultBrush defaultBrush = DefaultBrush::Black,
		  float strokeWidth = 1.f);
	void AlphaDrawLine(POINTFLOAT start, POINTFLOAT end, DefaultBrush defaultBrush, float alpha, float strokeWidth);
	void DrawRectangle( RECT rc,  D2D1::ColorF::Enum color,  float alpha,
		 float strokeWidth = 1.f);
	void DrawRectangle( RECT  rc,  DefaultBrush defaultBrush = DefaultBrush::Black,
		  float strokeWidth = 1.f);
	void DrawEllipse( POINTFLOAT origin,  float radius,  DefaultBrush defaultBrush = DefaultBrush::Black,
		   float strokeWidth = 1.f);
	void DrawEllipse( POINTFLOAT origin,  float radius,  D2D1::ColorF::Enum color,  float alpha,
		  float strokeWidth = 1.f);
	void FillRectangle( RECT rc,   D2D1::ColorF::Enum color,  float alpha);
	void FillRectangle( RECT rc,  DefaultBrush defaultBrush = DefaultBrush::Black);
	void FiilEllipse( POINTFLOAT origin,  float radius,  D2D1::ColorF::Enum color,  float alpha);
	void FiilEllipse( POINTFLOAT origin,  float radius,  DefaultBrush brush = DefaultBrush::Black);
};
