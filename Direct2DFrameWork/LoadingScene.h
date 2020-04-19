#pragma once
#include "GameNode.h"
#include "Loading.h"

class LoadingScene : public GameNode
{
private:
	Loading* _loading;

public:
	void Init(void);
	void Release(void);
	void Update(void);
	void Render(void);

	//�ε��̹��� �Լ�(�̰��� �̹����� ���� �־��)
	void LoadingImage();
	//�ε����� �Լ�(�̰��� ���带 ���� �־��)
	void LoadingSound();

	LoadingScene();
	~LoadingScene();
};

