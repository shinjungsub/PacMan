#pragma once
#include "GameObject.h"
#include "AStar.h"
class Enemy : public GameObject
{
public:
	//���ʹ� ���� enum
	enum class State
	{
		Confined,	//�����ִ� ����
		Ghost,		//���� ����
		Follow		//���󰡴� ����
	};
	AStar* _aStar;	//A* Ŭ����
private:
	State _state;	//���ʹ� ���� ����
private:
	vector<vector<tagAstar>> _tile;	//Ÿ�� ���� �޾ƿ� ����
	POINT _index;					//���ʹ� �ε���
	POINT _indexPrev;				//���ʹ� ���� �ε���
	POINT _playerPos;				//�÷��̾� ��ǥ(A*�� ���)
	int _imageFrameX;				//���ʹ� �̹��� ������ X
	int _imageCount;				//�̹��� ī��Ʈ ����
	int _speed;						//���ʹ� ���ǵ�
	bool _isSpreadLeg;				//�ٸ� ������ �ִ� �̹����ΰ�?
public:
	Enemy();
	Enemy(string name, POINT pos, State state);
	~Enemy();

	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;

	//Get �Լ�
	float GetSpeed() { return _speed; }
	State GetState() { return _state; }

	//Set �Լ�
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
	//�̹��� ������ ������Ʈ
	void UpdateImageFrame(void);

	bool CheckObjectInTile(Direction::Enum direction);
	void OrderTheGhostToTurn(void);
};

