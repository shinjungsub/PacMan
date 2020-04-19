// Direct2DFrameWork.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "Direct2DFrameWork.h"
#include "MainGame.h"
//=============================================================
//	## �������� ���ش� ##
//=============================================================
HWND		_hWnd;
HINSTANCE	_hInstance;
POINT		_ptMouse = { 0, 0 };

MainGame* _mainGame = nullptr;

//=============================================================
//	## �Լ� ���漱�� ##
//=============================================================
LRESULT WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
void SetWindowSize(int x, int y, int width, int height);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	//���ΰ��� Ŭ���� �����Ҵ�
	_mainGame = new MainGame;

	//�ν��Ͻ��� ���������� ��Ƶд�
	_hInstance = hInstance;

	//WNDCLASS : �������� ������ �����ϱ� ���� ����ü

	//������ ����ü ���� �� �ʱ�ȭ
	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;											//Ŭ���� ���� �޸�
	wndClass.cbWndExtra = 0;											//������ ���� �޸�
	wndClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);		//��׶���
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);						//���콺 Ŀ��
	wndClass.hIcon = LoadIcon(_hInstance, MAKEINTRESOURCE(IDI_PM));		//������
	wndClass.hInstance = hInstance;										//�ν��Ͻ�
	wndClass.lpfnWndProc = (WNDPROC)WndProc;							//������ ���ν���
	wndClass.lpszClassName = WINNAME;									//Ŭ���� �̸�
	wndClass.lpszMenuName = NULL;										//�޴��̸�
	wndClass.style = CS_HREDRAW | CS_VREDRAW;							//������ ��Ÿ��

	RegisterClass(&wndClass);

	//������â ����
	_hWnd = CreateWindow(
		WINNAME,				//������ Ŭ������ �̸�
		WINNAME,				//������ Ÿ��Ʋ�� �̸�
		WINSTYLE,				//������ ��Ÿ��
		WINSTARTX,				//������ ȭ����ǥ X
		WINSTARTY,				//������ ȭ����ǥ Y
		WINSIZEX,				//������ ȭ�� ����ũ��
		WINSIZEY,				//������ ȭ�� ����ũ��
		NULL,					//�θ� ������
		(HMENU)NULL,			//�޴��ڵ�
		hInstance,				//�ν��Ͻ� ����
		NULL					//������ �� �ڽ� �����츦 �����ϸ� �������ְ� �׷��� ������ NULL
	);

	//Ŭ���̾�Ʈ ������ ����� ��Ȯ�� ����ش�
	SetWindowSize(WINSTARTX, WINSTARTY, WINSIZEX, WINSIZEY);

	//ȭ�鿡 ������ �����ش�
	ShowWindow(_hWnd, nCmdShow);
	//���ӿ� ���߿� ����Ѵ�

	//MSG : �ü������ �����ϴ� �޼��� ������ �����ϱ� ���� ����ü
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

	//���ΰ��� Ŭ���� ����
	_mainGame->Release();
	SAFE_DELETE(_mainGame);

	//������ Ŭ���� ��� ����
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

	//���� ������ ũ�� ����
	AdjustWindowRect(&rc, WINSTYLE, false);

	//�� ��Ʈ ������ ������ Ŭ���̾�Ʈ ������ ����
	SetWindowPos(_hWnd, NULL, x, y, (rc.right - rc.left), (rc.bottom - rc.top),
		SWP_NOZORDER | SWP_NOMOVE);
}
