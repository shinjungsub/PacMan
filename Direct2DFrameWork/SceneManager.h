#pragma once
#include "SingletonBase.h"
//=============================================================
//	## sceneManager ## (���Ŵ���)
//=============================================================

class SceneManager : public SingletonBase<SceneManager>
{
private:
	typedef map<string,class GameNode*> mSceneList;
	typedef map<string,class GameNode*>::iterator miSceneList;

private:
	mSceneList _mSceneList;		//������ ��(ȭ��)���� ��Ƶ� ��
	class GameNode* _currentScene;	//�����

public:
	SceneManager();
	~SceneManager();

	void Update(void);
	void Render(void);

	//���߰�
	class GameNode* AddScene(string sceneName,class GameNode* scene);
	//������ (�ش�� �ε�)
	HRESULT LoadScene(string sceneName);
};

