#include "stdafx.h"
#include "SceneManager.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
	//�������ι��
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
	//������� �����ϸ� �ش��ϴ� ������� ������Ʈ�ض�
	if (_currentScene) _currentScene->Update();
}

void SceneManager::Render(void)
{
	//������� �����ϸ� �ش��ϴ� ������� �����ض�
	if (_currentScene) _currentScene->Render();
}

GameNode * SceneManager::AddScene(string sceneName, GameNode * scene)
{
	//���� ������ �׳� �� ����
	if (!scene) return NULL;

	//���� ������ �ʿ� ���
	_mSceneList.insert(make_pair(sceneName, scene));
	//mSceneList.insert(pair<string, gameNode*>(sceneName, scene));

	return scene;
}

HRESULT SceneManager::LoadScene(string sceneName)
{
	miSceneList find = _mSceneList.find(sceneName);

	//��ã�Ҵٸ� E_FAIL
	if (find == _mSceneList.end()) return E_FAIL;

	//�ٲٷ��� ���� ������� ���ٸ� E_FAIL
	if (find->second == _currentScene) return E_FAIL;

	//������� ������ ������ ������ ���� ����ó�� ����
	find->second->Init();
	{
		_currentScene = find->second;
		return S_OK;
	}

	return E_FAIL;
}
