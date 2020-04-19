#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
	//ĳ���� ����
	enum class State
	{
		None,
		Start,
		Run,
		Die
	};
private:
	State _state;			//ĳ���� ���� ����
	State _statePrev;		//ĳ���� ���� ����
private:
	int _imageFrameX;		//�÷��̾� �̹���������X
	int _imageCount;		//�̹��� ������ �ٲ�� �ӵ�
	POINT _index;			//���� �ε���
	POINT _indexPrev;		//���� �ε���
	int _speed;				//�÷��̾� ���ǵ�

	bool _isDie;			//�׾��°�?
	bool _isYum;			//���� ������ �� 0, ���� �ٹ������� 1 //�̹��� �����ӿ� ���
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

	//�÷��̾� �̵�
	void Move(void);
	//�÷��̾� Ű ����
	void KeyController(void);
	//�̹��� ������ ������Ʈ
	void UpdateImageFrame(void);
};

