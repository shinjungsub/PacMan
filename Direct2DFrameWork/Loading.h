#pragma once
#include "GameNode.h"

//=============================================================
//	## loadItem ## (�ε������ Ŭ����)
//=============================================================

//�ε������ ����
enum LOAD_KIND
{
	LOAD_KIND_IMAGE,			//�Ϲ� �̹���
	LOAD_KIND_FRAMEIMAGE,		//������ �̹���
	LOAD_KIND_SOUND,			//����
	LOAD_KIND_END
};

//�̹��� ���ҽ� ����ü
struct tagImageResource
{
	string keyName;				//�̹��� Ű��
	wstring fileName;			//�����̸�
	int maxFrameX, maxFrameY;	//������ x,y
};

//���� ���ҽ� ����ü
struct tagSoundResource
{
	string keyName;				//���� Ű��
	string fileName;			//�����̸�
	bool bgm;					//��׶��� �����ΰ�?
	bool loop;					//���� ��ų���ΰ�?
};

class loadItem
{
private:
	LOAD_KIND _kind;
	tagImageResource _imageResource;
	tagSoundResource _soundResource;

public:

	//�̹��� ���� �ʱ�ȭ(�ֻ��)
	HRESULT init(string keyName, wstring fileName);
	//������ �̹��� ���� �ʱ�ȭ(�ֻ��)
	HRESULT init(string keyName, wstring fileName, int frameX, int frameY);
	//���� ���� �ʱ�ȭ(�ֻ��)
	HRESULT init(string keyName, string fileName, bool bgm, bool loop);

	//�ε������� ���� ��������
	LOAD_KIND getLoadingKind() { return _kind; }
	//�̹��� ���ҽ� ��������
	tagImageResource getImageResource() { return _imageResource; }
	//���� ���ҽ� ��������
	tagSoundResource getSoundResource() { return _soundResource; }
	loadItem() {}
	~loadItem() {}
};

class Loading : public GameNode
{
private:
	//�ε� ������ Ŭ������ ���� ���� �� �ݺ���
	typedef vector<loadItem*> arrLoadItem;
	typedef vector<loadItem*>::iterator arrLoadItemIter;

private:
	arrLoadItem _vLoadItem;

	//�̺κ��� �ε�ȭ�鿡�� ����� �̹����� �ε���
	Image* _background;
	int _currentGauge;

public:
	void Init(void);
	void Release(void);
	void Update(void);
	void Render(void);

	//�̹��� ���� �ʱ�ȭ(�ֻ��)
	void loadImage(string keyName, wstring fileName);
	//������ �̹��� ���� �ʱ�ȭ(�ֻ��)
	void loadFrameImage(string keyName, wstring fileName, int maxFrameX, int maxFrameY);
	//���� ���� �ʱ�ȭ
	void loadSound(string keyName, string fileName, bool bgm, bool loop);

	/*�߿���*/
	//�ε��Ϸ� �Ƴ�? (�ε��Ϸ��� ȭ����ȯ)
	BOOL loadingDone();

	//�ε������ Ŭ������ ���� ���� ��������
	arrLoadItem getLoadItem() { return _vLoadItem; }
	int getCurrentGauge() { return _currentGauge; }
public:
	Loading() {}
	~Loading() {}
};

