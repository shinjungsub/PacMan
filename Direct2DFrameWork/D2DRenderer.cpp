#include "stdafx.h"
#include "D2DRenderer.h"

/*****************************************************
## D2DRenderer ##
*******************************************************/
D2DRenderer::D2DRenderer()
	:mD2DFactory(nullptr), mD2DRenderTarget(nullptr), mDWFactory(nullptr)
{
	this->CreateFactory();
	this->CreateRenderTarget();
	this->CreateDefaultBrush();
	this->AddTextFormat(L"맑은고딕");

	mD2DRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_ALIASED);
}

/*****************************************************
## ~D2DRenderer ##
*******************************************************/
D2DRenderer::~D2DRenderer()
{
	for (UINT i = 0; i < (UINT)DefaultBrush::End; ++i)
		SAFE_RELEASE(mDefaultBrushList[i]);

	SAFE_RELEASE(mD2DRenderTarget);
	SAFE_RELEASE(mD2DRenderTarget);
}
/*****************************************************
## BeginRender ##
*******************************************************/
void D2DRenderer::BeginRender()
{
	//윈도우가 가려져있는지 체크
	D2D1_WINDOW_STATE eWindowState = this->mD2DRenderTarget->CheckWindowState();
	if (!(eWindowState & D2D1_WINDOW_STATE_OCCLUDED))
	{
		mD2DRenderTarget->BeginDraw();
		mD2DRenderTarget->Clear();
		//mD2DRenderTarget->SetTransform(D2D1::IdentityMatrix());
	}
}
/*****************************************************
## EndRender ##
*******************************************************/
void D2DRenderer::EndRender()
{
#ifdef _DEBUG
	HRESULT hr = mD2DRenderTarget->EndDraw();
	assert(SUCCEEDED(hr));
#else
	mD2DRenderTarget->EndDraw();
#endif
}

/*****************************************************
## CreateFactory ##
*******************************************************/
void D2DRenderer::CreateFactory()
{
	//D2D 팩토리 생성
	//싱글쓰레드타입으로 
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &mD2DFactory);
#ifdef _DEBUG
	assert(SUCCEEDED(hr));
#endif
	//DWriteFactory 생성
	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(*&mDWFactory),
		(IUnknown**)&mDWFactory
	);
#ifdef _DEBUG
	assert(SUCCEEDED(hr));
#endif
}
/*****************************************************
## CreateRenderTarget ##
*******************************************************/
void D2DRenderer::CreateRenderTarget()
{
	RECT rc;
	GetClientRect(_hWnd, &rc);

	D2D1_RENDER_TARGET_PROPERTIES dxRTProperties = D2D1::RenderTargetProperties();
	D2D1_SIZE_U dxSize = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	//렌더타겟 생성
	HRESULT hr = this->mD2DFactory->CreateHwndRenderTarget(dxRTProperties,
		D2D1::HwndRenderTargetProperties(_hWnd, dxSize),
		&this->mD2DRenderTarget);
#ifdef _DEBUG
	assert(SUCCEEDED(hr));
#endif
}
/*****************************************************
## CreateDefaultBrush ##
*******************************************************/
void D2DRenderer::CreateDefaultBrush()
{
	this->mD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black, 1.0f), &mDefaultBrushList[(UINT)DefaultBrush::Black]);
	this->mD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White, 1.0f), &mDefaultBrushList[(UINT)DefaultBrush::White]);
	this->mD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow, 1.0f), &mDefaultBrushList[(UINT)DefaultBrush::Yellow]);
	this->mD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red, 1.0f), &mDefaultBrushList[(UINT)DefaultBrush::Red]);
	this->mD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 1.0f), &mDefaultBrushList[(UINT)DefaultBrush::Blue]);
	this->mD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green, 1.0f), &mDefaultBrushList[(UINT)DefaultBrush::Green]);
	this->mD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray, 1.0f), &mDefaultBrushList[(UINT)DefaultBrush::Gray]);
}
/*****************************************************
## AddTextFormat ##
@@ wstring font : 폰트명
@@ float defaultSize : 폰트 사이즈 = 1.f
*******************************************************/
void D2DRenderer::AddTextFormat( wstring font,  float defaultSize)
{
	HRESULT hr;
	IDWriteTextFormat* format = NULL;

	hr = this->mDWFactory->CreateTextFormat(
		font.c_str(),
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		defaultSize,
		L"ko",
		&format
	);
#ifdef _DEBUG
	assert(SUCCEEDED(hr));
#endif

	this->mFontList.insert(make_pair(font, format));
}


