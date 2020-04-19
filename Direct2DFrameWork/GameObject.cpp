#include "stdafx.h"
#include "GameObject.h"

#include <functional>
#include <unordered_map>

/*************************************************************************
## GameObject ##
**************************************************************************/
GameObject::GameObject()
	:_name(""), _pivot(Pivot::CENTER), _position({ 0, 0 }), _mainSize({ 0, 0 }), _isActive(true), _isLive(true), _isStart(false)
{
	this->UpdateMainRect();
}
/*************************************************************************
## GameObject ##
@@ string name : ��ü �̸�
@@ Vector2 pos : ��ü ��ǥ
@@ Vector2 size : ��ü ������
@@ Pivot::Enum pivot : ��ü �Ǻ�
**************************************************************************/
GameObject::GameObject(string name, POINT pos, POINT mainSize, Pivot::Enum pivot)
	:_name(name), _position(pos), _mainSize(mainSize), _pivot(pivot), _isActive(true), _isLive(true), _isStart(false)
{
	this->UpdateMainRect();
}

GameObject::GameObject(const RECT rect)
	: _name(""), _pivot(Pivot::LEFT_TOP), _isActive(true), _isLive(true), _isStart(false)
{
	this->_mainRect = rect;
	this->_position = { _mainRect.left, _mainRect.top };
	this->_mainSize = { _mainRect.right - _mainRect.left, _mainRect.bottom - _mainRect.top };
}

/*************************************************************************
## ~GameObject ##
**************************************************************************/
GameObject::~GameObject()
{
	this->_callbackList.clear();
	this->_reserveMessageList.clear();
}

void GameObject::Init()
{
}

/*************************************************************************
## Release ##
**************************************************************************/
void GameObject::Release()
{
	_reserveMessageList.clear();
}
/*************************************************************************
## Update ##
���� �޼����� �ִٸ� ����ð��� ��ٰ� �ð��� �ٵǾ��ٸ� �ش� �޼����� �Լ��� ȣ��
**************************************************************************/
void GameObject::Update()
{
	float deltaTime = TIMEMANAGER->getElapedTime();
	for (UINT i = 0; i < _reserveMessageList.size(); ++i)
	{
		//����� �޼����� ����ð��� deltaTime��ŭ ���ش�
		_reserveMessageList[i].delayTime -= deltaTime;
		//���� ����� �ð��� �ٵǾ��ٸ� 
		if (_reserveMessageList[i].delayTime <= 0.f)
		{
			//�ش� �޼����� �Լ��� ��������ش�. 
			this->SendCallbackMessage(_reserveMessageList[i]);
			_reserveMessageList.erase(_reserveMessageList.begin() + i--);
		}
	}
}
void GameObject::Render()
{
	if (KEYMANAGER->IsToggleKey(VK_F1))
	{
		D2DRENDERER->DrawRectangle(_mainRect, D2DRenderer::DefaultBrush::Red, true);
	}
}
/*************************************************************************
## SendCallbackMessage ##
@@ TagMessage message : �ش� ��ü�� ���� �޼���
**************************************************************************/
void GameObject::SendCallbackMessage(const TagMessage message)
{
	//���� ������ Ÿ���� ���ٸ� 
	if (message.delayTime <= 0.0f)
	{
		//�ٷ� �ش� �޼����� �̸��� ���� �Լ��� ã�Ƽ� ��������ش�. 
		CallbackHashmapIter iter = _callbackList.find(message.name);
		if (iter != _callbackList.end())
		{
			iter->second(message);
		}
	}
	//���� �޼����� ������ Ÿ���� �ִٸ�
	else
	{
		//���� �޼�������Ʈ�� �־�д�. 
		//GameObject�� ������Ʈ���� ���� �޼����� �ð��� ��ٰ� �ð��� �ٵǾ��ٸ� ȣ��
		_reserveMessageList.push_back(message);
	}
}
/*************************************************************************
## AddCallbackMessage ##
�޼��� ��ſ� ����� �Լ��� �̸� ����Ѵ�.
@@ string name : ����� �Լ��� �̸�
@@ function<void(TagMessage)> func : ����� �Լ�
**************************************************************************/
void GameObject::AddCallbackMessage(const string name, const function<void(struct TagMessage)> func)
{
	if (_callbackList.find(name) == _callbackList.end())
	{
		_callbackList.insert(make_pair(name, func));
	}
}

bool GameObject::MainRectCollision(RECT rc)
{
	RECT rcInter;

	if (IntersectRect(&rcInter, &rc, &_mainRect) == false) return false;

	int interW = rcInter.right - rcInter.left;
	int interH = rcInter.bottom - rcInter.top;

	if (interW > interH)
	{
		//��
		if (rcInter.top == rc.top)
		{
			SetPosition({ _position.x, _position.y - interH });
		}
		//�Ʒ�
		else if (rcInter.bottom == rc.bottom)
		{
			SetPosition({ _position.x, _position.y + interH });
		}
	}
	else
	{
		if (rcInter.left == rc.left)
		{
			SetPosition({ _position.x - interW, _position.y });
		}
		else if (rcInter.right == rc.right)
		{
			SetPosition({ _position.x + interW, _position.y });
		}
	}
	return true;
}

/*************************************************************************
## SetPivot ##
@@ Pivot::Enum pivot : �Ǻ�
**************************************************************************/
void GameObject::SetPivot(const Pivot::Enum pivot)
{
	this->_pivot = pivot;
	this->UpdateMainRect();
}
/*************************************************************************
## SetPosition ##
@@ Vector2 pos : �ٲ��� ��ǥ
**************************************************************************/
void GameObject::SetPosition(const POINT position)
{
	this->_position = position;
	this->UpdateMainRect();
}
/*************************************************************************
## SetMainSize ##
@@ Vector2 size : �ٲ��� ������
**************************************************************************/
void GameObject::SetMainSize(const POINT size)
{
	this->_mainSize = size;
	this->UpdateMainRect();
}
/*************************************************************************
## SetActive ##
Ȱ�����¸� �ٲ��ְ� Ȱ��,��Ȱ���ɶ��� ����Ǿ�� �� �Լ��� ȣ��
@@ bool b : Ȱ�� ����
**************************************************************************/
void GameObject::SetActive(const bool b)
{
	this->_isActive = b;
	if (_isActive)
		this->Enable();
	else
		this->Disable();
}
void GameObject::SetMainRect(const RECT rect)
{
	this->_mainRect = rect;
}
/*************************************************************************
## UpdateMainRect ##
�Ǻ��� ���� ��Ʈ�� ������ �������ش�.
**************************************************************************/
void GameObject::UpdateMainRect()
{
	switch (_pivot)
	{
	case Pivot::LEFT_TOP:
		this->_mainRect = Figure::RectMake(_position, _mainSize);
		break;
	case Pivot::CENTER:
		this->_mainRect = Figure::RectMakeCenter(_position, _mainSize);
		break;
	case Pivot::BOTTOM:
		this->_mainRect = Figure::RectMakeBottom(_position, _mainSize);
		break;
	default:
		break;
	}
}