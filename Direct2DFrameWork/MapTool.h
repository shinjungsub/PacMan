#pragma once
#include "GameNode.h"
#include "tileNode.h"

#define BUTTONSIZEX 100
#define BUTTONSIZEY 50


class MapTool : public GameNode
{
private:
	//=======================================
	// 맵에 따라 다른 타일들
	//=======================================
	tagTile _tiles[TILENUMY][TILENUMX];						//Game Map
	tagSampleTile _sampleTile[SAMPLETILEY][SAMPLETILEX];	//Sample Tile

	//=======================================
	// 샘플타일에 드래그 후 그대로 붙일 때 씀
	//=======================================
	INDEX _sampleDragStart;	//샘플타일 드래그 시작 인덱스
	INDEX _sampleDragEnd;	//샘플타일 드래그 끝 인덱스
	int _sampleDragNumX;	//샘플타일 드래그 가로 개수
	int _sampleDragNumY;	//샘플타일 드래그 세로 개수

	//=======================================
	// 타일 하나를 타일에 여러개 한꺼번에 붙일때 씀
	//=======================================
	INDEX _tileDragStart;	//타일 드래그 시작 인덱스
	INDEX _tileDragEnd;		//타일 드래그 끝 인덱스
	int _tileDragNumX;		//타일 드래그 가로 개수
	int _tileDragNumY;		//타일 드래그 세로 개수
	bool _isTileDrag;		//타일 드래그 하는가
	bool _isDragStart;		//드래그 시작
	
	//=======================================
	// 맵틀 버튼에 쓸 렉트들
	//=======================================
	RECT _rcInterface;	//인터페이스
	RECT _rcSave;		//제작한 맵 저장
	RECT _rcLoad;		//저장한 맵 불러오기
	RECT _rcEraser;		//오브젝트 지우기
	RECT _rcObject;		//오브젝트 설치

	//=======================================
	// enum 변수
	//=======================================
	CTRL _ctrlSelect;
	TILETYPE _tileType;

	//=======================================
	// 인덱스 변수
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

	//맵툴세팅
	void maptoolSetup();
	//버튼 클릭
	void clickButton();
	//맵에 타일 깔기
	void setMap();
	//타일에 드래그 하기
	void tileDrag();
	//타일 save
	void save();
	//타일 load
	void load();
	
	MapTool() {}
	~MapTool() {}
};
