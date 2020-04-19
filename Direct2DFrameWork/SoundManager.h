#pragma once
#include "SingletonBase.h"
/*FMOD 사용하기 위해 라이브러리 추가*/
#include "Sounds/inc/fmod.hpp"
#pragma comment (lib, "Sounds/lib/fmodex_vc.lib")
using namespace FMOD;
//=============================================================
//	## soundManager ## (사운드 매니져)
//=============================================================

//사용할 사운드버퍼 (사운드갯수가 30개가 넘어가면 크기를 더 늘려줘야 한다)
#define SOUNDBUFFER 2000
class SoundManager : public SingletonBase<SoundManager>
{
private:
	typedef map<string, Sound**> arrSound;
	typedef map<string, Sound**>::iterator arrSoundIter;

private:
	System* _system;		//시스템 클래스
	Sound** _sound;			//사운드 클래스
	Channel** _channel;		//채널 클래스

	arrSound _mTotalSound;	//맵에 담아둘 사운드들

	float _volume;

public:
	HRESULT Init(void);
	void Release(void);
	void Update(void);

	//사운드 추가(키값, 파일이름, BGM?, 루프시킬거냐?)
	void AddSound(string keyName, string soundName, bool bgm = false, bool loop = false);

	//사운드 재생
	void Play(string keyName); //0.0f(Min) ~ 1.0f(Max)
	//사운드 랜덤 재생
	void Play(const string &keyName, const int &minNum,const int &maxNum);
	//사운드 정지
	void Stop(string keyName);
	//사운드 일시정지
	void Pause(string keyName);
	//사운드 다시재생
	void Resume(string keyName);

	//플레이 중이냐?
	bool IsPlaySound(string keyName);
	//플레이 중이냐?
	bool IsPlaySound(string keyName, int minNum, int maxNum);
	//일시정지 중이냐?
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

