// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>
#include <time.h>
#include <assert.h>
#include <math.h>
#include <vector>
#include <map>
#include <unordered_map>
#include <functional>
#include <stack>
#include <fstream>
#include <algorithm>

using namespace std;
/**********************************************************************
## Library  ##
***********************************************************************/
//���̷�Ʈ2D ���̺귯��
#pragma comment(lib, "d2d1.lib")
#include <D2D1.h>
#include <d2d1helper.h>

//���̷�Ʈ Write ���̺귯�� (�ؽ�Ʈ)
#pragma comment(lib, "dwrite.lib")
#include <dwrite.h>

//WIC ���̺귯��
#pragma comment(lib, "WindowsCodecs.lib")
#include <wincodec.h>

/*************************************************************
## CastingInt ##
@@ n : ����,�Ǽ� �� ����

n�� int������ ��ȯ (float)1.0f�ϴ°Ŷ� ����
*************************************************************/
#define CastingInt(n) static_cast<int>(n)
/*************************************************************
## CastingFloat ##
@@ n : ����,�Ǽ� ��

n�� float������ ��ȯ
*************************************************************/
#define CastingFloat(n) static_cast<float>(n)
/**********************************************************************
## ��ƿ ##
***********************************************************************/
//���а��� Ŭ������
#include "DefineHeader.h"
#include "EnumStructHeader.h"
#include "Math.h"
#include "Vector2.h"
#include "Figure.h"

#include "tileNode.h"
#include "GameNode.h"
#include "StringHelper.h"
/**********************************************************************
## �Ŵ��� ##
***********************************************************************/
#include "D2DRenderer.h"
#include "ImageManager.h"
#include "KeyManager.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "RandomFunction.h"
#include "ObjectManager.h"
#include "SoundManager.h"

#define D2DRENDERER D2DRenderer::Get()
#define IMAGEMANAGER ImageManager::Get()
#define KEYMANAGER KeyManager::Get()
#define SCENEMANAGER SceneManager::Get()
#define TIMEMANAGER TimeManager::Get()
#define RND RandomFunction::Get()
#define OBJECTMANAGER ObjectManager::Get()
#define SOUNDMANAGER SoundManager::Get()

/**********************************************************************
## ���� ���� ##
***********************************************************************/
extern HINSTANCE	_hInstance;
extern HWND			_hWnd;
extern POINT		_ptMouse;