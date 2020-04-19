#pragma once
#include "SingletonBase.h"
/*FMOD ����ϱ� ���� ���̺귯�� �߰�*/
#include "Sounds/inc/fmod.hpp"
#pragma comment (lib, "Sounds/lib/fmodex_vc.lib")
using namespace FMOD;
//=============================================================
//	## soundManager ## (���� �Ŵ���)
//=============================================================

//����� ������� (���尹���� 30���� �Ѿ�� ũ�⸦ �� �÷���� �Ѵ�)
#define SOUNDBUFFER 2000
class SoundManager : public SingletonBase<SoundManager>
{
private:
	typedef map<string, Sound**> arrSound;
	typedef map<string, Sound**>::iterator arrSoundIter;

private:
	System* _system;		//�ý��� Ŭ����
	Sound** _sound;			//���� Ŭ����
	Channel** _channel;		//ä�� Ŭ����

	arrSound _mTotalSound;	//�ʿ� ��Ƶ� �����

	float _volume;

public:
	HRESULT Init(void);
	void Release(void);
	void Update(void);

	//���� �߰�(Ű��, �����̸�, BGM?, ������ų�ų�?)
	void AddSound(string keyName, string soundName, bool bgm = false, bool loop = false);

	//���� ���
	void Play(string keyName); //0.0f(Min) ~ 1.0f(Max)
	//���� ���� ���
	void Play(const string &keyName, const int &minNum,const int &maxNum);
	//���� ����
	void Stop(string keyName);
	//���� �Ͻ�����
	void Pause(string keyName);
	//���� �ٽ����
	void Resume(string keyName);

	//�÷��� ���̳�?
	bool IsPlaySound(string keyName);
	//�÷��� ���̳�?
	bool IsPlaySound(string keyName, int minNum, int maxNum);
	//�Ͻ����� ���̳�?
	bool IsPauseSound(string keyName);

	float GetVolume() { return _volume; }
	void SetVolume(float volume)
	{
		_volume = volume;
		UINT size = _mTotalSound.size();
		for (int i = 0; i < size; i++)
		{
			_channel[i]->setVolume(volume);
		}
	}

	void AllSoundStop()
	{
		UINT size = _mTotalSound.size();
		for (int i = 0; i < size; i++)
		{
			_channel[i]->stop();
		}
	}
public:
	SoundManager() :_system(NULL), _sound(NULL), _channel(NULL) {}
	~SoundManager() {}
};

