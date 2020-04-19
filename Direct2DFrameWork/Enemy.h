#pragma once
#include "GameObject.h"
#include "AStar.h"
class Enemy : public GameObject
{
public:
	//에너미 상태 enum
	enum class State
	{
		Confined,	//갇혀있는 상태
		Ghost,		//유령 상태
		Follow		//따라가는 상태
	};
	AStar* _aStar;	//A* 클래스
private:
	State _state;	//에너미 현재 상태
private:
	vector<vector<tagAstar>> _tile;	//타일 정보 받아올 벡터
	POINT _index;					//에너미 인덱스
	POINT _indexPrev;				//에너미 과거 인덱스
	POINT _playerPos;				//플레이어 좌표(A*에 사용)
	int _imageFrameX;				//에너미 이미지 프레임 X
	int _imageCount;				//이미지 카운트 변수
	int _speed;						//에너미 스피드
	bool _isSpreadLeg;				//다리 벌리고 있는 이미지인가?
public:
	Enemy();
	Enemy(string name, POINT pos, State state);
	~Enemy();

	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;

	//Get 함수
	float GetSpeed() { return _speed; }
	State GetState() { return _state; }

	//Set 함수
	void SetSpeed(float speed) { _speed = speed; }
	void SetState(State state)
	{
		_state = state;
		if (_state == State::Ghost)
		{
			_image = IMAGEMANAGER->FindImage("enemy_ghost");
			_speed = 1;
		}
		else
		{
			_image = IMAGEMANAGER->FindImage("enemy_" + _name);
			_speed = _state == State::Confined ? 1 : 2;
		}
	}
	void SetPlayerPos(POINT playerPos) { _playerPos = playerPos; }
	void SetTile(vector<vector<tagAstar>> tile) { _tile = tile; }

	void Move(void);
	//이미지 프레임 업데이트
	void UpdateImageFrame(void);

	bool CheckObjectInTile(Direction::Enum direction);
	void OrderTheGhostToTurn(void);
};

