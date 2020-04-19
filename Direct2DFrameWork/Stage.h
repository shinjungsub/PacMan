#pragma once
#include "GameNode.h"

class Stage : public GameNode
{
private:
	typedef struct tagRanking
	{
		string initial;		//���� �����Ϳ� �� �̴ϼ�
		int score;			//���� �����Ϳ� �� ����
		
	}tagScore;

	struct tagUI
	{
		RECT rc;
		POINT pos;
		POINT size;
		POINT maxSize;
	};
	
private:
	tagTile _tiles[TILENUMY][TILENUMX];		//������ ���� Ÿ�� ������ �޾ƿ� �迭
	vector<vector<tagAstar>> _vvTileInfo;	//A*�� ���� Ÿ�� ������ ���� ����
	class Image* _tileImage;				//Ÿ���̹���
	class Image* _alphabetImage;			//���ĺ�
	class Image* _numberImage;				//���� �̹���
	class Enemy* _enemy[4];					//���ʹ� 4����
	class Player* _player;					//�÷��̾�
	vector<class Item*> _vItem;				//�����۵�
private:
	vector<tagRanking> _vRanking;
	int _score;				//���� ���ھ�
	tagScore _highScore;	//���� ���� ���ھ�
	float _ghostCount;		//���ʹ̰� ��Ʈ�� ������ �� �ǵ��ƿ��� �ð� üũ�ϴ� ���� (5�ʰ� �Ǹ� �ǵ��� ������ ����)
	float _addEnemyCount;	//�����ִ� ���ʹ̰� �߰��Ǵ� �ð��� üũ�ϴ� ���� (7�ʸ��� �Ѹ����� �߰�)
	
	bool _isGhost;			//���ʹ̰� ��Ʈ�� �����ΰ�
	bool _isStart;			//������ ���� �Ǿ��°�
	bool _isEnd;			//������ �����°�

private:
	/************************
		�������̽� ���� ����
	************************/
	tagUI _interface;		//�������̽�
	tagUI _initial[3];		//����� �̴ϼ� �ڽ�

	int _initialNum[3];		//����� �̴ϼ�
	int _currentInitial;	//���� �� ��° �̴ϼ� ����ϰ� �ִ���

	float _zoom;			//�������̽� ũ�⸦ Ű��� ���� ����

	bool _isShow;			//�������̽��� ���� ���̴� ���ΰ�
	bool _isRank;			//��ŷ�� ���̴� ���ΰ�
public:
	void Init()override;
	void Release()override;
	void Update()override;
	void Render()override;
	
	//�Ű������� ���� ������Ʈ�κ��� �Ű������� ���� ������ Ÿ���� ��ȯ�ϴ� �Լ�
	tagTile TileInDirection(GameObject* object, Direction::Enum direction);
	//�Ű������� ���� ������Ʈ�� Ÿ�ϰ� �浹�ϴ��� üũ�ϰ� �������ϴ� �������� �� �� �ִ��� üũ�ϴ� �Լ�
	void CollisionOfGameObjectsWithTiles(GameObject* object);
	//�÷��̾�� �ٸ� ���ӿ�����Ʈ���� �浹
	void CollisionOfGamebjectWithPlayer(void);
	//�����ִ� ���ʹ̸� �߰��ϴ� �Լ�
	void AddEnemy(Enemy* enemy);
	//���� ���� Ű ����
	void KeySetting(void);

	//���� ���ھ� ����
	void CurrentScoreRender();
	//�ְ� ���ھ� ����
	void HighScoreRender();
	//�����̳� �����⸦ �Ϸ��� � Ű�� �������ϴ��� ������ �������� �ϴܿ� ����
	void KeySettingRender();
	
	//�ٽý��� �Լ�
	void ReStart();
	//�̴ϼ� ���
	void RecordInitial();
	//��ŷ ����
	void SaveRanking();
	//��ŷ �ҷ�����
	void LoadRanking();

	void UpdateRankingInterface();
	void RenderRankingInterface();

	//���ϴ� ��ġ�� ���ĺ� �̹��� ����
	void AlphabetRender(float x, float y, char c, POINTFLOAT size = { 12.0f, 12.0f });
	//���ھ� ���� ����
	void ScoreNumRender(int number, int startX, int startY, float size = 12.0f, Pivot::Enum pivot = Pivot::LEFT_TOP);

	Stage() {}
	~Stage() {}
};

