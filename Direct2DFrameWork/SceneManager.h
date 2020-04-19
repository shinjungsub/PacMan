#pragma once
#include "SingletonBase.h"
//=============================================================
//	## sceneManager ## (¾À¸Å´ÏÁ®)
//=============================================================

class SceneManager : public SingletonBase<SceneManager>
{
private:
	typedef map<string,class GameNode*> mSceneList;
	typedef map<string,class GameNode*>::iterator miSceneList;

private:
	mSceneList _mSceneList;		//°¢°¢ÀÇ ¾À(È­¸é)µéÀ» ´ã¾ÆµÑ ¸Ê
	class GameNode* _currentScene;	//ÇöÀç¾À

public:
	SceneManager();
	~SceneManager();

	void Update(void);
	void Render(void);

	//¾ÀÃß°¡
	class GameNode* AddScene(string sceneName,class GameNode* scene);
	//¾Àº¯°æ (ÇØ´ç¾À ·Îµå)
	HRESULT LoadScene(string sceneName);
};

