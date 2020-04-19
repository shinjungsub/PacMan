#pragma once
#include "GameNode.h"

//=============================================================
//	## loadItem ## (로드아이템 클래스)
//=============================================================

//로드아이템 종류
enum LOAD_KIND
{
	LOAD_KIND_IMAGE,			//일반 이미지
	LOAD_KIND_FRAMEIMAGE,		//프레임 이미지
	LOAD_KIND_SOUND,			//사운드
	LOAD_KIND_END
};

//이미지 리소스 구조체
struct tagImageResource
{
	string keyName;				//이미지 키값
	wstring fileName;			//파일이름
	int maxFrameX, maxFrameY;	//프레임 x,y
};

//사운드 리소스 구조체
struct tagSoundResource
{
	string keyName;				//사운드 키값
	string fileName;			//파일이름
	bool bgm;					//백그라운드 뮤직인가?
	bool loop;					//루프 시킬것인가?
};

class loadItem
{
private:
	LOAD_KIND _kind;
	tagImageResource _imageResource;
	tagSoundResource _soundResource;

public:

	//이미지 파일 초기화(주사용)
	HRESULT init(string keyName, wstring fileName);
	//프레임 이미지 파일 초기화(주사용)
	HRESULT init(string keyName, wstring fileName, int frameX, int frameY);
	//사운드 파일 초기화(주사용)
	HRESULT init(string keyName, string fileName, bool bgm, bool loop);

	//로딩아이템 종류 가져오기
	LOAD_KIND getLoadingKind() { return _kind; }
	//이미지 리소스 가져오기
	tagImageResource getImageResource() { return _imageResource; }
	//사운드 리소스 가져오기
	tagSoundResource getSoundResource() { return _soundResource; }
	loadItem() {}
	~loadItem() {}
};

class Loading : public GameNode
{
private:
	//로드 아이템 클래스를 담을 벡터 및 반복자
	typedef vector<loadItem*> arrLoadItem;
	typedef vector<loadItem*>::iterator arrLoadItemIter;

private:
	arrLoadItem _vLoadItem;

	//이부분은 로딩화면에서 사용할 이미지와 로딩바
	Image* _background;
	int _currentGauge;

public:
	void Init(void);
	void Release(void);
	void Update(void);
	void Render(void);

	//이미지 파일 초기화(주사용)
	void loadImage(string keyName, wstring fileName);
	//프레임 이미지 파일 초기화(주사용)
	void loadFrameImage(string keyName, wstring fileName, int maxFrameX, int maxFrameY);
	//사운드 파일 초기화
	void loadSound(string keyName, string fileName, bool bgm, bool loop);

	/*중요함*/
	//로딩완료 됐냐? (로딩완료후 화면전환)
	BOOL loadingDone();

	//로드아이템 클래스를 담은 벡터 가져오기
	arrLoadItem getLoadItem() { return _vLoadItem; }
	int getCurrentGauge() { return _currentGauge; }
public:
	Loading() {}
	~Loading() {}
};

