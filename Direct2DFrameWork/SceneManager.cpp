#include "stdafx.h"
#include "SceneManager.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
	//정적정인방법
	miSceneList iter = _mSceneList.begin();
	for (; iter != _mSceneList.end();)
	{
		if (iter->second != NULL)
		{
			if (iter->second == _currentScene) iter->second->Release();
			SAFE_DELETE(iter->second);
			iter = _mSceneList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	_mSceneList.clear();
}

void SceneManager::Update(void)
{
	//현재씬이 존재하면 해당하는 현재씬을 업데이트해라
	if (_currentScene) _currentScene->Update();
}

void SceneManager::Render(void)
{
	//현재씬이 존재하면 해당하는 현재씬을 렌더해라
	if (_currentScene) _currentScene->Render();
}

GameNode * SceneManager::AddScene(string sceneName, GameNode * scene)
{
	//씬이 없으면 그냥 널 리턴
	if (!scene) return NULL;

	//씬이 있으면 맵에 담기
	_mSceneList.insert(make_pair(sceneName, scene));
	//mSceneList.insert(pair<string, gameNode*>(sceneName, scene));

	return scene;
}

HRESULT SceneManager::LoadScene(string sceneName)
{
	miSceneList find = _mSceneList.find(sceneName);

	//못찾았다면 E_FAIL
	if (find == _mSceneList.end()) return E_FAIL;

	//바꾸려는 씬과 현재씬이 같다면 E_FAIL
	if (find->second == _currentScene) return E_FAIL;

	//여기까지 왔으면 무제가 없으니 씬을 변경처리 하자
	find->second->Init();
	{
		_currentScene = find->second;
		return S_OK;
	}

	return E_FAIL;
}
