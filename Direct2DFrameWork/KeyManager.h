#pragma once
#include "SingletonBase.h"
#define KEYMAX 256
class KeyManager : public SingletonBase<KeyManager>
{
private:
	bool _keyUp[KEYMAX];
	bool _keyDown[KEYMAX];
public:
	//Ű�� �ѹ��� ���ȳ�?
	bool IsOnceKeyDown(int key);
	//Ű�� �ѹ� ������ �����?
	bool IsOnceKeyUp(int key);
	//Ű�� ��� ���� �ֳ�?
	bool IsStayKeyDown(int key);
	//���Ű��?
	bool IsToggleKey(int key);

	KeyManager();
	~KeyManager();
};

