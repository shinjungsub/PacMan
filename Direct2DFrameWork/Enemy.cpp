#include "stdafx.h"
#include "Enemy.h"


Enemy::Enemy()
{
}

Enemy::Enemy(string name, POINT pos, State state)
	:GameObject(name, pos, {12, 12}, Pivot::CENTER)
{
	SetState(state);
	_aStar = new AStar;
}


Enemy::~Enemy()
{
}

void Enemy::Init()
{
	_aStar->SetTotalList(_tile);
	if (_state == State::Confined)
	{
		_direction = Direction::Up;
		_intendedDirection = Direction::Down;
	}
	else
	{
		_direction = Direction::Left;
		_intendedDirection = Direction::Left;
	}
	_index = { _position.x / TILESIZE, (_position.y - 120) / TILESIZE };
	_indexPrev = {0,0};
	_imageFrameX = 0;
	_imageCount = 0;
	_isSpreadLeg = false;
}

void Enemy::Release()
{
	delete _aStar;
}

void Enemy::Update()
{
	_index = { _position.x / TILESIZE, (_position.y - 120) / TILESIZE };
	this->Move();
	this->UpdateImageFrame();
}

void Enemy::Render()
{
	_image->SetSize({ 22, 22 });
	_image->FrameRender(_position.x, _position.y, _imageFrameX, 0, Pivot::CENTER);
}

//에너미의 움직임을 통제하는 함수
void Enemy::Move(void)
{
	switch (_state)
	{
	case Enemy::State::Confined:	//갇혀있을 때는 위아래로 왔다갔다만 함
	{
		if (_direction == Direction::Up)
		{
			if (CheckObjectInTile(Direction::Up))
				_direction = Direction::Down;
		}
		else
		{
			if (CheckObjectInTile(Direction::Down))
				_direction = Direction::Up;
		}
		break;
	}
	case Enemy::State::Ghost:		//유령상태일 때는 자동으로 돌아댕김
	{
		OrderTheGhostToTurn();
		break;
	}
	case Enemy::State::Follow:		//A*를 사용하여 플레이어 따라다님
	{
		//플레이어와 속도가 같아서 에너미의 인덱스가 변경되면 플레이어도 변경 될 것이므로
		//인덱스가 바뀔 때마다 재탐색
		if (_index.x != _indexPrev.x || _index.y != _indexPrev.y)
		{
			_aStar->SetTiles(_position, _playerPos);
			_aStar->PathFinder(_index);
			_intendedDirection = _aStar->GetDirection();
			_indexPrev = _index;
		}
		break;
	}
	default:
		break;
	}
	if (_isChangeDirection)
	{
		_direction = _intendedDirection;
		_isChangeDirection = false;
	}
	switch (_direction)
	{
	case Direction::Right:
		SetPosition({ _position.x + _speed, _position.y });
		break;
	case Direction::Down:
		SetPosition({ _position.x, _position.y + _speed });
		break;
	case Direction::Left:
		SetPosition({ _position.x - _speed, _position.y });
		break;
	case Direction::Up:
		SetPosition({ _position.x, _position.y - _speed });
		break;
	default:
		break;
	}
}

void Enemy::UpdateImageFrame(void)
{
	//이미지가 2개씩 짝지어서 방향별로 우하좌상 순으로 있음 그래서 그냥 bool로 이미지 프레임 처리
	_imageCount++;
	if (_imageCount > 5)
	{
		_isSpreadLeg = !_isSpreadLeg;
		_imageCount = 0;
	}

	int frameX = _isSpreadLeg;
	if (_state != State::Ghost)
	{
		switch (_direction)
		{
		case Direction::Right:
			break;
		case Direction::Down:
			frameX += 2;
			break;
		case Direction::Left:
			frameX += 4;
			break;
		case Direction::Up:
			frameX += 6;
			break;
		default:
			break;
		}
	}
	
	_imageFrameX = frameX;
}

bool Enemy::CheckObjectInTile(Direction::Enum direction)
{
	INDEX objectIndex = { (int)(_position.x / TILESIZE), (int)((_position.y - 120.0f) / TILESIZE) };
	tagAstar tile = _tile[objectIndex.y][objectIndex.x];
	switch (direction)
	{
	case Direction::Right:
		tile = _tile[objectIndex.y][objectIndex.x + 1];
		break;
	case Direction::Down:
		tile = _tile[objectIndex.y + 1][objectIndex.x];
		break;
	case Direction::Left:
		tile = _tile[objectIndex.y][objectIndex.x - 1];
		break;
	case Direction::Up:
		tile = _tile[objectIndex.y - 1][objectIndex.x];
		break;
	default:
		break;
	}
	return tile.haveObject;
}

void Enemy::OrderTheGhostToTurn(void)
{
	if (_state == Enemy::State::Follow)
		return;

	bool isFront = false;
	bool isLeft = false;
	bool isRight = false;
	isFront = !CheckObjectInTile(_direction);
	switch (_direction)
	{
	case Direction::Right:	//위, 아래
		isLeft = !CheckObjectInTile(Direction::Up);
		isRight = !CheckObjectInTile(Direction::Down);
		break;
	case Direction::Down:	//오른쪽, 왼쪽
		isLeft = !CheckObjectInTile(Direction::Right);
		isRight = !CheckObjectInTile(Direction::Left);
		break;
	case Direction::Left:	//아래, 위
		isLeft = !CheckObjectInTile(Direction::Down);
		isRight = !CheckObjectInTile(Direction::Up);
		break;
	case Direction::Up:		//왼쪽, 오른쪽
		isLeft = !CheckObjectInTile(Direction::Left);
		isRight = !CheckObjectInTile(Direction::Right);
		break;
	default:
		break;
	}
	//가는 방향 왼쪽 오른쪽 모두 갈 수 있는 곳일 때
	if (isFront && isLeft && isRight)
	{
		int num = Math::Random(0, 3);
		if (num == 0)
			_intendedDirection = _direction;
		else
			_intendedDirection = 
			num == 1 ?
			(Direction::Enum)(_direction == Direction::Right ? 3 : (int)_direction - 1) :
			(Direction::Enum)(_direction == Direction::Up ? 0 : (int)_direction + 1);
	}
	//일부분만 갈 수 있는 방향일 때
	else if (isFront || isLeft || isRight)
	{
		int num = isFront + isLeft + isRight;
		if (num == 2)	//2군데만 true
		{
			int randNum = Math::Random(0, 2);
			if (isFront)
			{
				if (isLeft)
					_intendedDirection =
					randNum == 0 ?
					_direction :
					(Direction::Enum)(_direction == Direction::Right ? 3 : (int)_direction - 1);
				else
					_intendedDirection =
					randNum == 0 ?
					_direction :
					(Direction::Enum)(_direction == Direction::Up ? 0 : (int)_direction + 1);
					
			}
			else
			{
				_intendedDirection = 
					randNum == 0 ?
					(Direction::Enum)(_direction == Direction::Right ? 3 : (int)_direction - 1) :
					(Direction::Enum)(_direction == Direction::Up ? 0 : (int)_direction + 1);
			}
		}
		else			//1군데만 true
		{
			if (isFront)
				_intendedDirection = _direction;
			else
				_intendedDirection =
				isLeft == true ?
				(Direction::Enum)(_direction == Direction::Right ? 3 : (int)_direction - 1) :
				(Direction::Enum)(_direction == Direction::Up ? 0 : (int)_direction + 1);
		}
	}
	//모두 막혀있을 때
	else
	{
		_intendedDirection =
			(int)_direction <= 1 ?
			(Direction::Enum)((int)_direction + 2) :
			(Direction::Enum)((int)_direction - 2);
	}
}
