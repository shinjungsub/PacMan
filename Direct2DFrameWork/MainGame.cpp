#include "stdafx.h"
#include "MainGame.h"
#include "LoadingScene.h"
#include "Stage.h"
#include "MapTool.h"

MainGame::MainGame()
{
}


MainGame::~MainGame()
{
}

void MainGame::Init()
{
	srand(time(NULL));
	SCENEMANAGER->AddScene("MapTool", new MapTool);
	SCENEMANAGER->AddScene("Loading", new LoadingScene);
	SCENEMANAGER->AddScene("Stage", new Stage);

	SCENEMANAGER->LoadScene("Loading");
	SOUNDMANAGER->Init();
}

void MainGame::Release()
{
	//싱글톤들 해제해주자
	ImageManager::Delete();
	D2DRenderer::Delete();
	KeyManager::Delete();
	SceneManager::Delete();
	TimeManager::Delete();
	RandomFunction::Delete();
	ObjectManager::Delete();
	SOUNDMANAGER->Release();
	SoundManager::Delete();
}

void MainGame::Update()
{
	SCENEMANAGER->Update();
	SOUNDMANAGER->Update();
}

void MainGame::Render()
{
	//윈도우가 가려져있는지 체크
	D2D1_WINDOW_STATE eWindowState = D2DRenderer::Get()->GetRenderTarget()->CheckWindowState();
	if (!(eWindowState & D2D1_WINDOW_STATE_OCCLUDED))
	{
		D2DRENDERER->BeginRender();
		/********************************************************/
		//여기서 그리시오

		SCENEMANAGER->Render();

		TIMEMANAGER->Render();
		/********************************************************/
		D2DRENDERER->EndRender();
	}
}
