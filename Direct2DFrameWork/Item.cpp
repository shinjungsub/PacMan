#include "stdafx.h"
#include "Item.h"

void Item::Init()
{
	_image = IMAGEMANAGER->FindImage("item");
}

void Item::Release()
{
}

void Item::Update()
{
}

void Item::Render()
{
	_image->SetSize({ 12, 12 });
	_image->FrameRender(_position.x, _position.y, _kind == Kind::SmallBall ? 0 : 2, 0);
}

void Item::ResetItem()
{
	int num = Math::Random(0, 300);
	if (num < 297) _kind = Kind::SmallBall;
	else _kind = Kind::BigBall;
	_point = _kind == Kind::SmallBall ? 50 : 100;
}
