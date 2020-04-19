#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
	//캐릭터 상태
	enum class State
	{
		None,
		Start,
		Run,
		Die
	};
private:
	State _state;			//캐릭터 현재 상태
	State _statePrev;		//캐릭터 과거 상태
private:
	int _imageFrameX;		//플레이어 이미지프레임X
	int _imageCount;		//이미지 프레임 바뀌는 속도
	POINT _index;			//현재 인덱스
	POINT _indexPrev;		//과거 인덱스
	int _speed;				//플레이어 스피드

	bool _isDie;			//죽었는가?
	bool _isYum;			//입을 벌렸을 때 0, 입을 다물었을때 1 //이미지 프레임에 사용
public:
	Player() {}
	Player(POINT pos) : GameObject("player", pos, { 12, 12 }, Pivot::CENTER) 
	{
		UpdateMainRect();
	}
	~Player() {}

	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;

	POINT GetIndex() { return _index; }
	POINT GetIndexPrev() { return _indexPrev; }
	void SetIndexPrev(POINT index) { _indexPrev = index; }
	void SetState(Player::State state) { _state = state; }

	//플레이어 이동
	void Move(void);
	//플레이어 키 조작
	void KeyController(void);
	//이미지 프레임 업데이트
	void UpdateImageFrame(void);
};

