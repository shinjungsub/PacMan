#include "stdafx.h"
#include "Loading.h"

//=============================================================
//	## loadItem ## (로드아이템 클래스)
//=============================================================

HRESULT loadItem::init(string keyName, wstring fileName)
{
	//로딩종류 초기화
	_kind = LOAD_KIND_IMAGE;
	//이미지 구조체 초기화
	_imageResource.keyName = keyName;
	_imageResource.fileName = fileName;

	return S_OK;
}

HRESULT loadItem::init(string keyName, wstring fileName, int maxFrameX, int maxFrameY)
{
	//로딩종류 초기화
	_kind = LOAD_KIND_FRAMEIMAGE;
	//이미지 구조체 초기화
	_imageResource.keyName = keyName;
	_imageResource.fileName = fileName;
	_imageResource.maxFrameX = maxFrameX;
	_imageResource.maxFrameY = maxFrameY;

	return S_OK;
}

HRESULT loadItem::init(string keyName, string fileName, bool bgm, bool loop)
{
	_kind = LOAD_KIND_SOUND;
	//사운드 구조체 초기화
	_soundResource.keyName = keyName;
	_soundResource.fileName = fileName;
	_soundResource.bgm = bgm;
	_soundResource.loop = loop;
	return S_OK;
}

//=============================================================
//	## loading ## (로딩 클래스)
//=============================================================
void Loading::Init(void)
{

	//현재 게이지
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
	//로딩완료됨
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
	//곧 사운드 배우고 난후 알아서...
	case LOAD_KIND_SOUND:
		tagSoundResource sound = item->getSoundResource();
		SOUNDMANAGER->AddSound(sound.keyName, sound.fileName, sound.bgm, sound.loop);
		break;
	}

	//현재 게이지 증가
	_currentGauge++;

	return 0;
}
