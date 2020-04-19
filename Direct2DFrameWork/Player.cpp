#include "stdafx.h"
#include "Player.h"

void Player::Init()
{	
	_image = IMAGEMANAGER->FindImage("player_start");

	_imageFrameX = 0;
	_imageCount = 0;

	_direction = Direction::Right;
	_intendedDirection = Direction::Right;
	
	_speed = 2;
	_isYum = false;
	_isChangeDirection = false;
	_isStart = false;
	_index = { (int)_position.x / TILESIZE, ((int)_position.y - 120) / TILESIZE };
	_indexPrev = { 0,0 };
	_state = State::Start;
	_statePrev = State::None;
}

void Player::Release()
{
}

void Player::Update()
{
	KeyController();

	Move();

	UpdateImageFrame();
	_index = { (int)_position.x / TILESIZE, ((int)_position.y - 120) / TILESIZE };
}

void Player::Render()
{
	if (_state == State::Start)
	{
		_image->SetSize({ 22, 22 });
		_image->Render(_position.x, _position.y);
	}
	else
	{
		if(_state == State::Run)
			_image->SetSize({ 22, 22 });
		else
			_image->SetSize({ 24, 24 });
		_image->FrameRender(_position.x, _position.y, _imageFrameX, 0, Pivot::CENTER);
	}
}

//플레이어 움직임 통제
void Player::Move(void)
{
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

//방향키
void Player::KeyController(void)
{
	if (KEYMANAGER->IsOnceKeyDown(VK_LEFT))
	{
		_intendedDirection = Direction::Left;
	}
	if (KEYMANAGER->IsOnceKeyDown(VK_UP))
	{
		_intendedDirection = Direction::Up;
	}
	if (KEYMANAGER->IsOnceKeyDown(VK_RIGHT))
	{
		_intendedDirection = Direction::Right;
	}
	if (KEYMANAGER->IsOnceKeyDown(VK_DOWN))
	{
		_intendedDirection = Direction::Down;
	}
}

//이미지 프레임 업데이트
void Player::UpdateImageFrame(void)
{
	if (_state != _statePrev)
	{
		_imageCount = 0;
		_imageFrameX = 0;
		_statePrev = _state;
	}
	switch (_state)
	{
	case Player::State::Start:
	{
		_image = IMAGEMANAGER->FindImage("player_start");
		_imageCount = 0;
		_imageFrameX = 0;
		break;
	}
	case Player::State::Run:
	{
		//이미지가 2개씩 짝지어서 방향별로 우하좌상 순으로 있음 그래서 그냥 bool로 이미지 프레임 처리
		_image = IMAGEMANAGER->FindImage("player_move");
		_imageCount++;
		if (_imageCount > 5)
		{
			_isYum = !_isYum;
			_imageCount = 0;
		}

		int frameX = !_isYum;
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
		_imageFrameX = frameX;
		break;
	}	
	case Player::State::Die:
	{
		_image = IMAGEMANAGER->FindImage("player_die");
		_imageCount++;
		if (_imageCount > 5)
		{
			_imageFrameX++;
			_imageCount = 0;
		}
		if (_imageFrameX >= _image->GetMaxFrameX())
			_isActive = false;
		break;
	}
	default:
		break;
	}
}

