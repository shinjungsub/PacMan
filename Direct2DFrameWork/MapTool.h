#pragma once
#include "GameNode.h"
#include "tileNode.h"

#define BUTTONSIZEX 100
#define BUTTONSIZEY 50


class MapTool : public GameNode
{
private:
	//=======================================
	// �ʿ� ���� �ٸ� Ÿ�ϵ�
	//=======================================
	tagTile _tiles[TILENUMY][TILENUMX];						//Game Map
	tagSampleTile _sampleTile[SAMPLETILEY][SAMPLETILEX];	//Sample Tile

	//=======================================
	// ����Ÿ�Ͽ� �巡�� �� �״�� ���� �� ��
	//=======================================
	INDEX _sampleDragStart;	//����Ÿ�� �巡�� ���� �ε���
	INDEX _sampleDragEnd;	//����Ÿ�� �巡�� �� �ε���
	int _sampleDragNumX;	//����Ÿ�� �巡�� ���� ����
	int _sampleDragNumY;	//����Ÿ�� �巡�� ���� ����

	//=======================================
	// Ÿ�� �ϳ��� Ÿ�Ͽ� ������ �Ѳ����� ���϶� ��
	//=======================================
	INDEX _tileDragStart;	//Ÿ�� �巡�� ���� �ε���
	INDEX _tileDragEnd;		//Ÿ�� �巡�� �� �ε���
	int _tileDragNumX;		//Ÿ�� �巡�� ���� ����
	int _tileDragNumY;		//Ÿ�� �巡�� ���� ����
	bool _isTileDrag;		//Ÿ�� �巡�� �ϴ°�
	bool _isDragStart;		//�巡�� ����
	
	//=======================================
	// ��Ʋ ��ư�� �� ��Ʈ��
	//=======================================
	RECT _rcInterface;	//�������̽�
	RECT _rcSave;		//������ �� ����
	RECT _rcLoad;		//������ �� �ҷ�����
	RECT _rcEraser;		//������Ʈ �����
	RECT _rcObject;		//������Ʈ ��ġ

	//=======================================
	// enum ����
	//=======================================
	CTRL _ctrlSelect;
	TILETYPE _tileType;

	//=======================================
	// �ε��� ����
	//=======================================
	INDEX _mouseIdx;

	//=======================================
	// 
	//=======================================
	char str[256];
	wstring wstr[256];
	class Image* _tileImage;

public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;

	//��������
	void maptoolSetup();
	//��ư Ŭ��
	void clickButton();
	//�ʿ� Ÿ�� ���
	void setMap();
	//Ÿ�Ͽ� �巡�� �ϱ�
	void tileDrag();
	//Ÿ�� save
	void save();
	//Ÿ�� load
	void load();
	
	MapTool() {}
	~MapTool() {}
};
