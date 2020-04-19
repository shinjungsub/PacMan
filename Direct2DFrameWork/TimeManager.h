#pragma once
#include "SingletonBase.h"
//=============================================================
//	## timeManager ## (Ÿ�� �Ŵ���)
//=============================================================

class TimeManager : public SingletonBase <TimeManager>
{
private:
	bool _isHardware;				//���� Ÿ�̸Ӹ� �����ϳ�?
	float _timeScale;				//�ð� �����
	float _timeElapsed;				//�������Ӵ� �����
	__int64 _curTime;				//����ð�
	__int64 _lastTime;				//�������ð�
	__int64 _periodFrequency;		//�ð��ֱ� (1�ʿ� ����̳� ī��Ʈ�Ǵ���)

	unsigned long _frameRate;		//FPS
	unsigned long _FPSFrameCount;	//FPS ī��Ʈ
	float _FPSTimeElapsed;			//FPS �����
	float _worldTime;				//��ü ����ð�

public:
	TimeManager();
	~TimeManager();
	void Start();
	void Update(float lockFPS);
	void Render();

	//�������Ӵ� ����ð� ��������
	float getElapedTime(void) { return _timeElapsed; }
	//��ü ����ð� ��������
	float getWorldTime(void) { return _worldTime; }
};

