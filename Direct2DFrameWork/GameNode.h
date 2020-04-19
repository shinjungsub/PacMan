#pragma once
#include "Image.h"
class GameNode
{
public:
	GameNode();
	virtual ~GameNode();

	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0; 
};

