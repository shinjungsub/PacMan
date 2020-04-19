// Direct2DFrameWork.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Direct2DFrameWork.h"
#include "MainGame.h"
//=============================================================
//	## 전역변수 써준다 ##
//=============================================================
HWND		_hWnd;
HINSTANCE	_hInstance;
POINT		_ptMouse = { 0, 0 };

MainGame* _mainGame = nullptr;

//=============================================================
//	## 함수 전방선언 ##
//=============================================================
LRESULT WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
void SetWindowSize(int x, int y, int width, int height);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	//메인게임 클래스 동적할당
	_mainGame = new MainGame;

	//인스턴스를 전역변수에 담아둔다
	_hInstance = hInstance;

	//WNDCLASS : 윈도우의 정보를 저장하기 위한 구조체

	//윈도우 구조체 선언 및 초기화
	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;											//클래스 여분 메모리
	wndClass.cbWndExtra = 0;											//윈도우 여분 메모리
	wndClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);		//백그라운드
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);						//마우스 커서
	wndClass.hIcon = LoadIcon(_hInstance, MAKEINTRESOURCE(IDI_PM));		//아이콘
	wndClass.hInstance = hInstance;										//인스턴스
	wndClass.lpfnWndProc = (WNDPROC)WndProc;							//윈도우 프로시져
	wndClass.lpszClassName = WINNAME;									//클래스 이름
	wndClass.lpszMenuName = NULL;										//메뉴이름
	wndClass.style = CS_HREDRAW | CS_VREDRAW;							//윈도우 스타일

	RegisterClass(&wndClass);

	//윈도우창 생성
	_hWnd = CreateWindow(
		WINNAME,				//윈도우 클래스의 이름
		WINNAME,				//윈도우 타이틀바 이름
		WINSTYLE,				//윈도우 스타일
		WINSTARTX,				//윈도우 화면좌표 X
		WINSTARTY,				//윈도우 화면좌표 Y
		WINSIZEX,				//윈도우 화면 가로크기
		WINSIZEY,				//윈도우 화면 세로크기
		NULL,					//부모 윈도우
		(HMENU)NULL,			//메뉴핸들
		hInstance,				//인스턴스 지정
		NULL					//윈도우 및 자식 윈도우를 생성하면 지정해주고 그렇지 않으면 NULL
	);

	//클라이언트 영역의 사이즈를 정확히 잡아준다
	SetWindowSize(WINSTARTX, WINSTARTY, WINSIZEX, WINSIZEY);

	//화면에 윈도우 보여준다
	ShowWindow(_hWnd, nCmdShow);
	//게임용 나중에 사용한다

	//MSG : 운영체제에서 발행하는 메세지 정보를 저장하기 위한 구조체
	MSG message;

	TimeManager::Get()->Start();
	_mainGame->Init();

	while (true)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT) break;
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			TIMEMANAGER->Update(60.0f);
			_mainGame->Update();
			_mainGame->Render();
		}
	}

	//메인게임 클래스 해제
	_mainGame->Release();
	SAFE_DELETE(_mainGame);

	//윈도우 클래스 등록 해제
	UnregisterClass(WINNAME, hInstance);

	return message.wParam;
}

LRESULT WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_MOUSEWHEEL:
	case WM_MOUSEMOVE:
		_ptMouse.x = LOWORD(lParam);
		_ptMouse.y = HIWORD(lParam);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostMessage(hWnd, WM_DESTROY, 0, 0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void SetWindowSize(int x, int y, int width, int height)
{
	RECT rc = { 0, 0, width, height };

	//실제 윈도우 크기 조정
	AdjustWindowRect(&rc, WINSTYLE, false);

	//위 렉트 정보로 윈도우 클라이언트 사이즈 세팅
	SetWindowPos(_hWnd, NULL, x, y, (rc.right - rc.left), (rc.bottom - rc.top),
		SWP_NOZORDER | SWP_NOMOVE);
}
