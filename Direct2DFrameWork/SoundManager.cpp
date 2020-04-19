#include "stdafx.h"
#include "SoundManager.h"

HRESULT SoundManager::Init(void)
{
	//FMOD �ý����� �����Ѵ�
	System_Create(&_system);

	//����, ä�μ� ����
	_system->init(SOUNDBUFFER, FMOD_INIT_NORMAL, 0);

	//����, ä�� Ŭ���� �����Ҵ�
	_sound = new Sound*[SOUNDBUFFER];
	_channel = new Channel*[SOUNDBUFFER];

	//�޸� �ʱ�ȭ
	memset(_sound, 0, sizeof(Sound*) * SOUNDBUFFER);
	memset(_channel, 0, sizeof(Channel*) * SOUNDBUFFER);

	_volume = 1.f;
	UINT size = _mTotalSound.size();
	for (int i = 0; i < size; i++)
	{
		_channel[i]->setVolume(1.f);
	}

	return S_OK;
}

void SoundManager::Release(void)
{
	//���� �� ä�� ����
	if (_sound != NULL || _channel != NULL)
	{
		for (int i = 0; i < SOUNDBUFFER; i++)
		{
			if (_channel != NULL)
			{
				if (_channel[i]) _channel[i]->stop();
			}
			if (_sound != NULL)
			{
				if (_sound[i]) _sound[i]->release();
			}
		}
	}

	//�޸� �����
	SAFE_DELETE_ARRAY(_sound);
	SAFE_DELETE_ARRAY(_channel);

	//�ý��� �ݱ�
	if (_system != NULL)
	{
		_system->release();
		_system->close();
	}
}

void SoundManager::Update(void)
{
	//���ΰ��ӿ��� ����Ŵ����� ������Ʈ ����� �Ѵ�
	//���� ���� �Ѱ������ �Ѵ�
	//���� �ý����� ������ ����ǰų�, ����� ������ ���带 �����ϴµ�
	//���� ���������� ������ �̷����� �ﰢ������ ó�����ش�
	_system->update();
}

void SoundManager::AddSound(string keyName, string soundName, bool bgm, bool loop)
{
	if (loop)//�Ϲ������� BGM
	{
		if (bgm)
		{
			//���� ������ �̸� ���ε��صΰ� ����Ѵ�
			//�Ѱ��� ���ϸ� �������
			_system->createStream(soundName.c_str(), FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSound.size()]);
		}
		else//���� ������
		{
			//���� ������ �׶��׶� �ǽð����� �ε��Ѵ�
			//���� ȿ������ ���� ũ�Ⱑ Ŭ��� ���۰� �����
			//�������� ���� ������� �����ϱ⶧���� ȿ������ �����ϴ�
			_system->createSound(soundName.c_str(), FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSound.size()]);
		}
	}
	else //�Ϲ������� ȿ����
	{
		_system->createSound(soundName.c_str(), FMOD_DEFAULT, 0, &_sound[_mTotalSound.size()]);
	}

	//�ʿ� ���带 Ű���� �Բ� ��Ƶд�
	_mTotalSound.insert(make_pair(keyName, &_sound[_mTotalSound.size()]));

}

void SoundManager::Play(string keyName)
{
	int count = 0;
	arrSoundIter iter = _mTotalSound.begin();
	for (iter; iter != _mTotalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//���� �÷���
			_system->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &_channel[count]);
		}
	}
}

void SoundManager::Play(const string &keyName, const int &minNum, const int &maxNum)
{
	string str = keyName;
	int randomNum = RND->Range(minNum, maxNum);
	string total = str + to_string(randomNum);
	int count = 0;
	arrSoundIter iter = _mTotalSound.begin();
	for (iter; iter != _mTotalSound.end(); ++iter, count++)
	{
		if (total == iter->first)
		{
			//���� �÷���
			_system->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &_channel[count]);
			_channel[count]->setVolume(_volume);
		}
	}
}

void SoundManager::Stop(string keyName)
{
	int count = 0;
	arrSoundIter iter = _mTotalSound.begin();
	for (iter; iter != _mTotalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//���� ����
			_channel[count]->stop();
		}
	}
}

void SoundManager::Pause(string keyName)
{
	int count = 0;
	arrSoundIter iter = _mTotalSound.begin();
	for (iter; iter != _mTotalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//���� �Ͻ�����
			_channel[count]->setPaused(true);
			break;
		}
	}
}

void SoundManager::Resume(string keyName)
{
	int count = 0;
	arrSoundIter iter = _mTotalSound.begin();
	for (iter; iter != _mTotalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//���� �ٽ����
			_channel[count]->setPaused(false);
			break;
		}
	}
}

bool SoundManager::IsPlaySound(string keyName)
{
	bool isPlay;
	int count = 0;
	arrSoundIter iter = _mTotalSound.begin();
	for (iter; iter != _mTotalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//���� �÷������̳�?
			_channel[count]->isPlaying(&isPlay);
			break;
		}
	}

	return isPlay;
}

bool SoundManager::IsPlaySound(string keyName, int minNum, int maxNum)
{
	string key = keyName;
	bool isPlay;
	
	for (int i = minNum; i < maxNum + 1; i++)
	{
		string total = key + to_string(i);
		int count = 0;
		arrSoundIter iter = _mTotalSound.begin();
		for (iter; iter != _mTotalSound.end(); ++iter, count++)
		{
			if (total == iter->first)
			{
				//���� �÷������̳�?
				_channel[count]->isPlaying(&isPlay);
				break;
			}
		}
		if (isPlay) return true;
	}

	return isPlay;
}

bool SoundManager::IsPauseSound(string keyName)
{
	bool isPause;
	int count = 0;
	arrSoundIter iter = _mTotalSound.begin();
	for (iter; iter != _mTotalSound.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//���� �÷������̳�?
			_channel[count]->isPlaying(&isPause);
			break;
		}
	}

	return isPause;
}