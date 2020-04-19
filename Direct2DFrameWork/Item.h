#pragma once
#include "GameObject.h"

class Item : public GameObject
{
public:
	enum class Kind
	{
		SmallBall,
		BigBall
	};
private:
	Kind _kind;
	int _point;
public:
	Item() {}
	Item(string name, POINT _pos) : GameObject(name, _pos, {12, 12}, Pivot::CENTER)
	{
		int num = Math::Random(0, 300);
		if (num < 297) _kind = Kind::SmallBall;
		else _kind = Kind::BigBall;
		_image = _kind == Kind::SmallBall ? IMAGEMANAGER->FindImage("smallBall") : IMAGEMANAGER->FindImage("bigBall");
		_point = _kind == Kind::SmallBall ? 50 : 100;
	}
	~Item() {}

	Kind GetKind() { return _kind; }
	int GetPoint() { return _point; }

	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;

	void ResetItem();
};
