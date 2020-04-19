#pragma once
#include "Image.h"
//����ȭ�� Ÿ��
#define TILESIZE 12				//Ÿ�� ���� ũ��
#define TILENUMX 28				//Ÿ�� X ����
#define TILENUMY 36				//Ÿ�� Y ����

//�̹��� Ÿ��
#define SAMPLETILESIZE 36		//sampleŸ�� ���� ũ��
#define SAMPLETILEX 11			//sampleŸ�� X������ ����
#define SAMPLETILEY 5			//sampleŸ�� Y������ ����

//�������� ����� ��Ʈ�� �̳ѹ�
enum class CTRL
{
	SAVE, LOAD,
	END
};

//Ÿ���� ��� ��ġ���� ������ ���ϴ� �̳ѹ�
enum class TILETYPE
{
	OBJECT, ERASER
};

//Ÿ�ϱ���ü
struct tagTile
{
	RECT rc;

	int objFrameX;
	int objFrameY;

	bool haveObject;
};

//���� Ÿ�� ����ü
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

//����Ÿ�� ����ü
struct INDEX
{
	int x, y;
};