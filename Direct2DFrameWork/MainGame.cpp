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
	//�̱���� ����������
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
	//�����찡 �������ִ��� üũ
	D2D1_WINDOW_STATE eWindowState = D2DRenderer::Get()->GetRenderTarget()->CheckWindowState();
	if (!(eWindowState & D2D1_WINDOW_STATE_OCCLUDED))
	{
		D2DRENDERER->BeginRender();
		/********************************************************/
		//���⼭ �׸��ÿ�

		SCENEMANAGER->Render();

		TIMEMANAGER->Render();
		/********************************************************/
		D2DRENDERER->EndRender();
	}
}
