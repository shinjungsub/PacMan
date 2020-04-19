#include "stdafx.h"
#include "LoadingScene.h"


void LoadingScene::Init(void)
{
	//로딩클래스 초기화
	_loading = new Loading;
	_loading->Init();

	//이미지 및 사운드 로딩
	this->LoadingImage();
	this->LoadingSound();
}

void LoadingScene::Release(void)
{
	//로딩클래스 해제
	_loading->Release();
	SAFE_DELETE(_loading);
}

void LoadingScene::Update(void)
{
	//로딩클래스 업데이트
	_loading->Update();

	//로딩완료후 화면(씬)변경
	if (_loading->loadingDone())
	{
		SCENEMANAGER->LoadScene("Stage");
	}
}

void LoadingScene::Render(void)
{
	//로딩클래스 렌더
	_loading->Render();

	float num = _loading->getLoadItem().size();
	float cur = _loading->getCurrentGauge();
	float per = cur / num * 100;
	char str[128];
	sprintf(str, "%.f %%", per);
	wstring w_str;
	for (int i = 0; i < 128; i++)
	{
		if (str[i] == '\0')
		{
			break;
		}
		w_str.push_back(str[i]);
	}
	D2DRENDERER->RenderText(WINSIZEX / 2.0f - 50.0f, WINSIZEY / 2.0f + 20.0f, w_str, 20, D2DRenderer::DefaultBrush::Red);

	if (_loading->getCurrentGauge() < _loading->getLoadItem().size())
	{
		if (_loading->getLoadItem()[_loading->getCurrentGauge()]->getLoadingKind() == LOAD_KIND::LOAD_KIND_IMAGE ||
			_loading->getLoadItem()[_loading->getCurrentGauge()]->getLoadingKind() == LOAD_KIND::LOAD_KIND_FRAMEIMAGE)
		{
			string str1 = ".png";
			string total;
			total = _loading->getLoadItem()[_loading->getCurrentGauge()]->getImageResource().keyName + str1;
			wstring w_str1;
			w_str1.assign(total.begin(), total.end());
			D2DRENDERER->RenderText(WINSIZEX / 2.0f - 50.0f, WINSIZEY / 2.0f - 20.0f, w_str1, 20, D2DRenderer::DefaultBrush::Red);
		}
		/*else if (_loading->getLoadItem()[_loading->getCurrentGauge()]->getLoadingKind() == LOAD_KIND::LOAD_KIND_SOUND)
		{
			string str2 = ".ogg";
			string total;
			total = _loading->getLoadItem()[_loading->getCurrentGauge()]->getSoundResource().keyName + str2;
			wstring w_str1;
			w_str1.assign(total.begin(), total.end());
			D2DRENDERER->RenderText(600, 330, w_str1, 20, D2DRenderer::DefaultBrush::Red);
		}*/
	}
}

void LoadingScene::LoadingImage()
{
	///////////////
	///* Enemy */
	///////////////
	_loading->loadFrameImage("enemy_blue", L"./Graphics/Enemy/pm_enemy_blue.png", 8, 1);
	_loading->loadFrameImage("enemy_pink", L"./Graphics/Enemy/pm_enemy_pink.png", 8, 1);
	_loading->loadFrameImage("enemy_yellow", L"./Graphics/Enemy/pm_enemy_yellow.png", 8, 1);
	_loading->loadFrameImage("enemy_red", L"./Graphics/Enemy/pm_enemy_red.png", 8, 1);
	_loading->loadFrameImage("enemy_dark", L"./Graphics/Enemy/pm_enemy_dark.png", 8, 1);
	_loading->loadFrameImage("enemy_ghost", L"./Graphics/Enemy/pm_enemy_ghost.png", 2, 1);
	_loading->loadFrameImage("enemy_blueGhost", L"./Graphics/Enemy/pm_enemy_blueGhost.png", 2, 1);

	///////////////
	///* Player */
	///////////////
	_loading->loadFrameImage("player_move", L"./Graphics/Player/pm_player_move.png", 8, 1);
	_loading->loadFrameImage("player_die", L"./Graphics/Player/pm_player_die.png", 11, 1);
	_loading->loadImage("player_start", L"./Graphics/Player/pm_player_start.png");

	///////////////
	///* UI */
	///////////////
	_loading->loadFrameImage("alphabet", L"./Graphics/UI/pm_alphabet.png", 26, 1);
	_loading->loadFrameImage("number", L"./Graphics/UI/pm_number.png", 10, 1);
	_loading->loadImage("recordInterface", L"./Graphics/UI/pm_recordInterface.png");
	_loading->loadImage("currentInitial", L"./Graphics/UI/pm_currentInitial.png");

	///////////////
	///* Item */
	///////////////
	_loading->loadFrameImage("item", L"./Graphics/Item/pm_item.png", 3, 1);

	///////////////
	///* MapTool */
	///////////////
	_loading->loadFrameImage("tile", L"./Graphics/MapTool/pm_maptool_tile.png", 11, 5);
	_loading->loadImage("maptool_interface", L"./Graphics/MapTool/pm_maptool_interface.png");
	_loading->loadImage("maptool_save", L"./Graphics/MapTool/pm_maptool_save.png");
	_loading->loadImage("maptool_save_click", L"./Graphics/MapTool/pm_maptool_save_click.png");
	_loading->loadImage("maptool_load", L"./Graphics/MapTool/pm_maptool_load.png");
	_loading->loadImage("maptool_load_click", L"./Graphics/MapTool/pm_maptool_load_click.png");
	_loading->loadImage("maptool_object", L"./Graphics/MapTool/pm_maptool_object.png");
	_loading->loadImage("maptool_object_click", L"./Graphics/MapTool/pm_maptool_object_click.png");
	_loading->loadImage("maptool_eraser", L"./Graphics/MapTool/pm_maptool_eraser.png");
	_loading->loadImage("maptool_eraser_click", L"./Graphics/MapTool/pm_maptool_eraser_click.png");
}

void LoadingScene::LoadingSound()
{
	//스타트씬
	_loading->loadSound("beginning", "./Sounds/pacman_beginning.wav", false, false);
	_loading->loadSound("chomp", "./Sounds/pacman_chomp.wav", false, false);
	_loading->loadSound("death", "./Sounds/pacman_death.wav", false, false);
	_loading->loadSound("eatfruit", "./Sounds/pacman_eatfruit.wav", false, false);
	_loading->loadSound("intermission", "./Sounds/pacman_intermission.wav", false, false);
}

LoadingScene::LoadingScene()
{
}


LoadingScene::~LoadingScene()
{
}
