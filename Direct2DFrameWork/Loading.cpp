#include "stdafx.h"
#include "Loading.h"

//=============================================================
//	## loadItem ## (�ε������ Ŭ����)
//=============================================================

HRESULT loadItem::init(string keyName, wstring fileName)
{
	//�ε����� �ʱ�ȭ
	_kind = LOAD_KIND_IMAGE;
	//�̹��� ����ü �ʱ�ȭ
	_imageResource.keyName = keyName;
	_imageResource.fileName = fileName;

	return S_OK;
}

HRESULT loadItem::init(string keyName, wstring fileName, int maxFrameX, int maxFrameY)
{
	//�ε����� �ʱ�ȭ
	_kind = LOAD_KIND_FRAMEIMAGE;
	//�̹��� ����ü �ʱ�ȭ
	_imageResource.keyName = keyName;
	_imageResource.fileName = fileName;
	_imageResource.maxFrameX = maxFrameX;
	_imageResource.maxFrameY = maxFrameY;

	return S_OK;
}

HRESULT loadItem::init(string keyName, string fileName, bool bgm, bool loop)
{
	_kind = LOAD_KIND_SOUND;
	//���� ����ü �ʱ�ȭ
	_soundResource.keyName = keyName;
	_soundResource.fileName = fileName;
	_soundResource.bgm = bgm;
	_soundResource.loop = loop;
	return S_OK;
}

//=============================================================
//	## loading ## (�ε� Ŭ����)
//=============================================================
void Loading::Init(void)
{

	//���� ������
	_currentGauge = 0;
}

void Loading::Release(void)
{
}

void Loading::Update(void)
{
}

void Loading::Render(void)
{
}

void Loading::loadImage(string keyName, wstring fileName)
{
	loadItem* item = new loadItem;
	item->init(keyName, fileName);
	_vLoadItem.push_back(item);
}

void Loading::loadFrameImage(string keyName, wstring fileName, int maxFrameX, int maxFrameY)
{
	loadItem* item = new loadItem;
	item->init(keyName, fileName, maxFrameX, maxFrameY);
	_vLoadItem.push_back(item);
}

void Loading::loadSound(string keyName, string fileName, bool bgm, bool loop)
{
	loadItem* item = new loadItem;
	item->init(keyName, fileName, bgm, loop);
	_vLoadItem.push_back(item);
}

BOOL Loading::loadingDone()
{
	//�ε��Ϸ��
	if (_currentGauge >= _vLoadItem.size())
	{
		return TRUE;
	}

	loadItem* item = _vLoadItem[_currentGauge];

	switch (item->getLoadingKind())
	{
	case LOAD_KIND_IMAGE:
	{
		tagImageResource img = item->getImageResource();
		IMAGEMANAGER->AddImage(img.keyName, img.fileName);
	}
	break;
	case LOAD_KIND_FRAMEIMAGE:
	{
		tagImageResource img = item->getImageResource();
		IMAGEMANAGER->AddFrameImage(img.keyName, img.fileName, img.maxFrameX, img.maxFrameY);
	}
	break;
	//�� ���� ���� ���� �˾Ƽ�...
	case LOAD_KIND_SOUND:
		tagSoundResource sound = item->getSoundResource();
		SOUNDMANAGER->AddSound(sound.keyName, sound.fileName, sound.bgm, sound.loop);
		break;
	}

	//���� ������ ����
	_currentGauge++;

	return 0;
}
