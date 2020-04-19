#pragma once
#include "SingletonBase.h"
#define KEYMAX 256
class KeyManager : public SingletonBase<KeyManager>
{
private:
	bool _keyUp[KEYMAX];
	bool _keyDown[KEYMAX];
public:
	//키가 한번만 눌렸냐?
	bool IsOnceKeyDown(int key);
	//키가 한번 눌렀다 띄었냐?
	bool IsOnceKeyUp(int key);
	//키가 계속 눌려 있냐?
	bool IsStayKeyDown(int key);
	//토글키냐?
	bool IsToggleKey(int key);

	KeyManager();
	~KeyManager();
};

