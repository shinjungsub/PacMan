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

	//로딩이미지 함수(이곳에 이미지를 전부 넣어라)
	void LoadingImage();
	//로딩사운드 함수(이곳에 사운드를 전부 넣어라)
	void LoadingSound();

	LoadingScene();
	~LoadingScene();
};

