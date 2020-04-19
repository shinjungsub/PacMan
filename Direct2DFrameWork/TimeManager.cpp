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
	//���� Ÿ�̸� �������� üũ
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&_periodFrequency))
	{
		//���� Ÿ�̸Ӹ� �����ϴϱ�
		_isHardware = true;
		//�ʴ� �ð� ���(1�� ��� ī���� �ϳ�)
		_timeScale = 1.0f / _periodFrequency;
		//�ʱ�ȭ ������ �ð��� ������ �ð�����
		QueryPerformanceCounter((LARGE_INTEGER*)&_lastTime);
	}
	else
	{
		//���� Ÿ�̸Ӹ� �������� �����ϱ�
		_isHardware = false;
		_timeScale = 1.0f / 1000.0f;
		_lastTime = timeGetTime();
	}

	//������ �ʱ�ȭ
	_frameRate = 0;
	_FPSFrameCount = 0;
	_FPSTimeElapsed = 0.0f;
	_worldTime = 0;
}

void TimeManager::Update(float lockFPS)
{
	if (_isHardware)
	{
		//����ð� ���
		QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);
	}
	else
	{
		//���� Ÿ�̸Ӹ� �������� �ʱ⶧���� �׳� time�Լ� ����Ѵ�
		_curTime = timeGetTime();
	}

	//�������� ������ �� ���
	_timeElapsed = (_curTime - _lastTime) * _timeScale;

	//������ ��Ű��
	if (lockFPS > 0.0f)
	{
		while (_timeElapsed < (1.0f / lockFPS))
		{
			if (_isHardware)
			{
				//����ð� ���
				QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);
			}
			else
			{
				//���� Ÿ�̸Ӹ� �������� �ʱ⶧���� �׳� time�Լ� ����Ѵ�
				_curTime = timeGetTime();
			}
			//�������� ������ �� ���
			_timeElapsed = (_curTime - _lastTime) * _timeScale;
		}
	}

	//���� �ð� ����
	_lastTime = _curTime;
	//������ �� ����
	_FPSTimeElapsed += _timeElapsed;
	//������ �ð� ����
	_worldTime += _timeElapsed;

	//������ �ʱ�ȭ�� 1�ʸ��� �����ϱ�
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
