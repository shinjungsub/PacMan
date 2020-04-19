// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
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
//다이렉트2D 라이브러리
#pragma comment(lib, "d2d1.lib")
#include <D2D1.h>
#include <d2d1helper.h>

//다이렉트 Write 라이브러리 (텍스트)
#pragma comment(lib, "dwrite.lib")
#include <dwrite.h>

//WIC 라이브러리
#pragma comment(lib, "WindowsCodecs.lib")
#include <wincodec.h>

/*************************************************************
## CastingInt ##
@@ n : 정수,실수 형 변수

n을 int형으로 변환 (float)1.0f하는거랑 같음
*************************************************************/
#define CastingInt(n) static_cast<int>(n)
/*************************************************************
## CastingFloat ##
@@ n : 정수,실수 형

n을 float형으로 변환
*************************************************************/
#define CastingFloat(n) static_cast<float>(n)
/**********************************************************************
## 유틸 ##
***********************************************************************/
//수학관련 클래스들
#include "DefineHeader.h"
#include "EnumStructHeader.h"
#include "Math.h"
#include "Vector2.h"
#include "Figure.h"

#include "tileNode.h"
#include "GameNode.h"
#include "StringHelper.h"
/**********************************************************************
## 매니져 ##
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
## 전역 변수 ##
***********************************************************************/
extern HINSTANCE	_hInstance;
extern HWND			_hWnd;
extern POINT		_ptMouse;