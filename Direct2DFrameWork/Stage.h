#pragma once
#include "GameNode.h"

class Stage : public GameNode
{
private:
	typedef struct tagRanking
	{
		string initial;		//순위 데이터에 들어갈 이니셜
		int score;			//순위 데이터에 들어갈 점수
		
	}tagScore;

	struct tagUI
	{
		RECT rc;
		POINT pos;
		POINT size;
		POINT maxSize;
	};
	
private:
	tagTile _tiles[TILENUMY][TILENUMX];		//맵툴로 만든 타일 정보를 받아올 배열
	vector<vector<tagAstar>> _vvTileInfo;	//A*를 위한 타일 정보를 담을 벡터
	class Image* _tileImage;				//타일이미지
	class Image* _alphabetImage;			//알파벳
	class Image* _numberImage;				//숫자 이미지
	class Enemy* _enemy[4];					//에너미 4마리
	class Player* _player;					//플레이어
	vector<class Item*> _vItem;				//아이템들
private:
	vector<tagRanking> _vRanking;
	int _score;				//현재 스코어
	tagScore _highScore;	//가장 높은 스코어
	float _ghostCount;		//에너미가 고스트로 변했을 때 되돌아오는 시간 체크하는 변수 (5초가 되면 되돌아 오도록 설정)
	float _addEnemyCount;	//갇혀있는 에너미가 추가되는 시간을 체크하는 변수 (7초마다 한마리씩 추가)
	
	bool _isGhost;			//에너미가 고스트인 상태인가
	bool _isStart;			//게임이 시작 되었는가
	bool _isEnd;			//게임이 끝났는가

private:
	/************************
		인터페이스 관련 변수
	************************/
	tagUI _interface;		//인터페이스
	tagUI _initial[3];		//등록할 이니셜 박스

	int _initialNum[3];		//등록할 이니셜
	int _currentInitial;	//현재 몇 번째 이니셜 등록하고 있는지

	float _zoom;			//인터페이스 크기를 키우고 줄일 변수

	bool _isShow;			//인터페이스가 지금 보이는 중인가
	bool _isRank;			//랭킹이 보이는 중인가
public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;
	
	//매개변수로 들어가는 오브젝트로부터 매개변수로 들어가는 방향의 타일을 반환하는 함수
	tagTile TileInDirection(GameObject* object, Direction::Enum direction);
	//매개변수로 들어가는 오브젝트가 타일과 충돌하는지 체크하고 가려고하는 방향으로 갈 수 있는지 체크하는 함수
	void CollisionOfGameObjectsWithTiles(GameObject* object);
	//플레이어와 다른 게임오브젝트들의 충돌
	void CollisionOfGamebjectWithPlayer(void);
	//갇혀있는 에너미를 추가하는 함수
	void AddEnemy(Enemy* enemy);
	//게임 관련 키 세팅
	void KeySetting(void);

	//현재 스코어 렌더
	void CurrentScoreRender();
	//최고 스코어 렌더
	void HighScoreRender();
	//시작이나 나가기를 하려면 어떤 키를 눌러야하는지 설명을 스테이지 하단에 렌더
	void KeySettingRender();
	
	//다시시작 함수
	void ReStart();
	//이니셜 기록
	void RecordInitial();
	//랭킹 저장
	void SaveRanking();
	//랭킹 불러오기
	void LoadRanking();

	void UpdateRankingInterface();
	void RenderRankingInterface();

	//원하는 위치에 알파벳 이미지 렌더
	void AlphabetRender(float x, float y, char c, POINTFLOAT size = { 12.0f, 12.0f });
	//스코어 점수 렌더
	void ScoreNumRender(int number, int startX, int startY, float size = 12.0f, Pivot::Enum pivot = Pivot::LEFT_TOP);

	Stage() {}
	~Stage() {}
};

