#include "stdafx.h"
#include "TimeManager.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
TimeManager::TimeManager()
{
	
}


TimeManager::~TimeManager()
{
}

void TimeManager::Start()
{
	//고성능 타이머 지원여부 체크
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&_periodFrequency))
	{
		//고성능 타이머를 지원하니까
		_isHardware = true;
		//초당 시간 계산(1초 몇번 카운팅 하냐)
		_timeScale = 1.0f / _periodFrequency;
		//초기화 시점의 시간을 마지막 시간으로
		QueryPerformanceCounter((LARGE_INTEGER*)&_lastTime);
	}
	else
	{
		//고성능 타이머를 지원하지 않으니까
		_isHardware = false;
		_timeScale = 1.0f / 1000.0f;
		_lastTime = timeGetTime();
	}

	//변수들 초기화
	_frameRate = 0;
	_FPSFrameCount = 0;
	_FPSTimeElapsed = 0.0f;
	_worldTime = 0;
}

void TimeManager::Update(float lockFPS)
{
	if (_isHardware)
	{
		//현재시간 얻기
		QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);
	}
	else
	{
		//고성능 타이머를 지원하지 않기때문에 그냥 time함수 사용한다
		_curTime = timeGetTime();
	}

	//한프레임 간격의 초 계산
	_timeElapsed = (_curTime - _lastTime) * _timeScale;

	//프레임 스키핑
	if (lockFPS > 0.0f)
	{
		while (_timeElapsed < (1.0f / lockFPS))
		{
			if (_isHardware)
			{
				//현재시간 얻기
				QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);
			}
			else
			{
				//고성능 타이머를 지원하지 않기때문에 그냥 time함수 사용한다
				_curTime = timeGetTime();
			}
			//한프레임 간격의 초 계산
			_timeElapsed = (_curTime - _lastTime) * _timeScale;
		}
	}

	//지난 시간 갱신
	_lastTime = _curTime;
	//프레임 초 누적
	_FPSTimeElapsed += _timeElapsed;
	//총지난 시간 누적
	_worldTime += _timeElapsed;

	//프레임 초기화를 1초마다 진행하기
	if (_FPSTimeElapsed > 1.0f)
	{
		_frameRate = _FPSFrameCount;
		_FPSFrameCount = 0;
		_FPSTimeElapsed = 0.0f;
	}

	_FPSFrameCount++;
}

void TimeManager::Render()
{
	D2DRENDERER->RenderText(5, 5, L"WorldTime : " + StringHelper::StringToWString(to_string(this->_worldTime)), 20, D2DRenderer::DefaultBrush::White);
	D2DRENDERER->RenderText(5, 30, L"DeltaTime : " + StringHelper::StringToWString(to_string(this->_timeElapsed)), 20, D2DRenderer::DefaultBrush::White);
	D2DRENDERER->RenderText(5, 55, L"FPS : " + StringHelper::StringToWString(to_string(this->_frameRate)), 20, D2DRenderer::DefaultBrush::White);
}
