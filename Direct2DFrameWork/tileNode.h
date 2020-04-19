#pragma once
#include "Image.h"
//게임화면 타일
#define TILESIZE 12				//타일 개당 크기
#define TILENUMX 28				//타일 X 개수
#define TILENUMY 36				//타일 Y 개수

//이미지 타일
#define SAMPLETILESIZE 36		//sample타일 개당 크기
#define SAMPLETILEX 11			//sample타일 X프레임 개수
#define SAMPLETILEY 5			//sample타일 Y프레임 개수

//맵툴에서 사용할 컨트롤 이넘문
enum class CTRL
{
	SAVE, LOAD,
	END
};

//타일을 어떻게 설치할지 지울지 정하는 이넘문
enum class TILETYPE
{
	OBJECT, ERASER
};

//타일구조체
struct tagTile
{
	RECT rc;

	int objFrameX;
	int objFrameY;

	bool haveObject;
};

//샘플 타일 구조체
struct tagSampleTile
{
	RECT rc;
	int frameX;
	int frameY;
};

struct tagAstar
{
	tagAstar* parentTile;

	POINT center;
	POINT index;

	int totalCost;
	int costFromStart;
	int costToEnd;

	bool isOpen;
	bool isClose;
	bool haveObject;

	void Init(int x, int y, bool object)
	{
		parentTile = nullptr;
		index = { x, y };
		totalCost = 0;
		costFromStart = 0;
		costToEnd = 0;
		isOpen = false;
		isClose = false;
		haveObject = object;
	}
};

//현재타일 구조체
struct INDEX
{
	int x, y;
};