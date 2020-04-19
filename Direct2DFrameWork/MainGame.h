#pragma once
#include "GameNode.h"

class MainGame : public GameNode
{
private:

public:
	MainGame();
	virtual ~MainGame();

	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;
};

