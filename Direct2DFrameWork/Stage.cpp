#include "stdafx.h"
#include "Stage.h"
#include "Player.h"
#include "Enemy.h"
#include "Item.h"

void Stage::Init()
{
	SOUNDMANAGER->Play("beginning");
	//�̹��� �ʱ�ȭ
	_tileImage = IMAGEMANAGER->FindImage("tile");
	_alphabetImage = IMAGEMANAGER->FindImage("alphabet");
	_numberImage = IMAGEMANAGER->FindImage("number");

	//���� �� �ð� üũ ���� �ʱ�ȭ
	_score = 0;
	LoadRanking();
	_ghostCount = 0.0f;
	_addEnemyCount = 0.0f;

	//bool ���� �ʱ�ȭ
	_isGhost = false;
	_isStart = false;
	_isEnd = false;

	//���ڵ� �������̽�
	_zoom = 0.0f;
	_currentInitial = 0;
	_isShow = false;
	_isRank = false;

	_interface.maxSize = { 320, 150 };
	_interface.pos = { (long)WINSIZEX / 2, (long)WINSIZEY / 2 };
	_interface.size = { (long)(_interface.maxSize.x * _zoom), (long)(_interface.maxSize.y * _zoom) };
	_interface.rc = Figure::RectMakeCenter(_interface.pos.x, _interface.pos.y, _interface.size.x, _interface.size.y);
	for (int i = 0; i < 3; i++)
	{
		_initial[i].maxSize = { 48, 48 };
		_initial[i].pos = { _interface.pos.x + (int)((i - 1) * 50 * _zoom), _interface.pos.y + (int)(30 * _zoom) };
		_initial[i].size = { (long)(_initial[i].maxSize.x * _zoom), (long)(_initial[i].maxSize.y * _zoom) };
		_initial[i].rc = Figure::RectMakeCenter(_initial[i].pos.x, _initial[i].pos.y, _initial[i].size.x, _initial[i].size.y);
		_initialNum[i] = 0;
	}

	//�� ���� �ҷ�����
	HANDLE file;
	DWORD read;
	file = CreateFile(L"./tile.map", GENERIC_READ, 0, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, _tiles, sizeof(tagTile) * TILENUMX * TILENUMY, &read, NULL);
	CloseHandle(file);

	//A*�� ���� Ÿ�� ���� �� Ÿ�� ������ ���� ������ ��ġ �ʱ�ȭ
	int itemNum = 0;
	for (int i = 0; i < TILENUMY; i++)
	{
		vector<tagAstar> vTileInfo;
		for (int j = 0; j < TILENUMX; j++)
		{
			tagAstar tileInfo;
			tileInfo.parentTile = nullptr;
			tileInfo.index = { j, i };
			tileInfo.center = { _tiles[i][j].rc.left + 6, _tiles[i][j].rc.top + 6 };
			tileInfo.haveObject = _tiles[i][j].haveObject;
			tileInfo.totalCost = 0;
			tileInfo.costToEnd = 0;
			tileInfo.costFromStart = 0;
			tileInfo.isOpen = false;
			tileInfo.isClose = false;
			vTileInfo.push_back(tileInfo);
			if (!((j > 10 && j < 17) && (i > 15 && i < 19)) && !tileInfo.haveObject)
			{
				itemNum++;
				OBJECTMANAGER->AddObject(ObjectType::Object, new Item("item" + to_string(itemNum), tileInfo.center));
				Item* item = (Item*)OBJECTMANAGER->FindObject(ObjectType::Object, "item" + to_string(itemNum));
				_vItem.push_back(item);
			}
		}
		_vvTileInfo.push_back(vTileInfo);
	}

	//������Ʈ�Ŵ����� �÷��̾� ��ü �߰�
	OBJECTMANAGER->AddObject(ObjectType::UpObject, new Player({ TILENUMX / 2 * 12, _tiles[26][0].rc.top + 6 }));
	_player = (Player*)OBJECTMANAGER->FindObject(ObjectType::UpObject, "player");

	//������Ʈ�Ŵ����� ���ʹ� ��ü �߰�
	OBJECTMANAGER->AddObject(ObjectType::UpObject, new Enemy("red", { TILENUMX / 2 * 12, _tiles[14][0].rc.top + 6 }, Enemy::State::Follow));
	OBJECTMANAGER->AddObject(ObjectType::UpObject, new Enemy("pink", { TILENUMX / 2 * 12 - 24, _tiles[17][0].rc.top + 6 }, Enemy::State::Confined));
	OBJECTMANAGER->AddObject(ObjectType::UpObject, new Enemy("blue", { TILENUMX / 2 * 12, _tiles[17][0].rc.top + 6 }, Enemy::State::Confined));
	OBJECTMANAGER->AddObject(ObjectType::UpObject, new Enemy("yellow", { TILENUMX / 2 * 12 + 24, _tiles[17][0].rc.top + 6 }, Enemy::State::Confined));
	_enemy[0] = (Enemy*)OBJECTMANAGER->FindObject(ObjectType::UpObject, "red");
	_enemy[1] = (Enemy*)OBJECTMANAGER->FindObject(ObjectType::UpObject, "pink");
	_enemy[2] = (Enemy*)OBJECTMANAGER->FindObject(ObjectType::UpObject, "blue");
	_enemy[3] = (Enemy*)OBJECTMANAGER->FindObject(ObjectType::UpObject, "yellow");

	//A* ����� ���� ���ʹ̿��� Ÿ�������� �÷��̾� ��ġ���� �Ѱ��ֱ�
	for (int i = 0; i < 4; i++)
	{
		_enemy[i]->SetTile(_vvTileInfo);
		_enemy[i]->SetPlayerPos(_player->GetPosition());
	}
	
	OBJECTMANAGER->Init();
}

void Stage::Release()
{
	OBJECTMANAGER->Release();
}

void Stage::Update()
{
	UpdateRankingInterface();
	KeySetting();
	if (_zoom > 0.0f) return;
	//���� ��Ȳ�϶��� �÷��̾� �״� �̹��� ������ ������Ʈ�� ����
	if (_isEnd)
	{
		_player->UpdateImageFrame();
		return;
	}
	//������ ��Ȳ�� �ƴҶ��� ����
	if (!_isStart) return;
	//������Ʈ�� ������Ʈ
	OBJECTMANAGER->Update();

	//�÷��̾�� ���ʹ� Ÿ�� �浹 üũ
	CollisionOfGameObjectsWithTiles(_player);
	for (int i = 0; i < 4; i++)
		CollisionOfGameObjectsWithTiles(_enemy[i]);

	//�÷��̾�� ������, ���ʹ� �浹
	CollisionOfGamebjectWithPlayer();

	//���ʹ̰� 9�ʸ��� �Ѹ����� �߰���.
	_addEnemyCount += TIMEMANAGER->getElapedTime();
	if (_addEnemyCount > 9.0f)
	{
		for (int i = 0; i < 4; i++)
		{
			if (_enemy[i]->GetState() == Enemy::State::Confined)
			{
				AddEnemy(_enemy[i]);
				_addEnemyCount = 0.0f;
				break;
			}
		}
	}
	//���ʹ̵��� ��Ʈ �����϶� �ð��� üũ�Ͽ� 5���Ŀ� ������� �ǵ�����
	if (_isGhost)
	{
		_ghostCount += TIMEMANAGER->getElapedTime();
		for (int i = 0; i < 4; i++)
			if (_enemy[i]->GetState() == Enemy::State::Follow)
				_enemy[i]->SetState(Enemy::State::Ghost);
		if (_ghostCount > 5.0f)
		{
			_isGhost = false;
			_ghostCount = 0.0f;
			for (int i = 0; i < 4; i++)
			{
				if (_enemy[i]->GetState() == Enemy::State::Ghost)
				{
					_enemy[i]->SetState(Enemy::State::Follow);
					//Ghost ������ �� ���ǵ尡 1�� ���ƴٴϴµ� �������� Ȧ���� ���� �� Follow�� ���°� �ٲ�� �ٸ� �������� �̵����� ���ϴ� ��찡 ���ܼ� ����ó�� ����
					if ((int)(_enemy[i]->GetPosition().x) % 2 == 1)
						_enemy[i]->SetPosition({ _enemy[i]->GetPosition().x + 1, _enemy[i]->GetPosition().y });
					if ((int)(_enemy[i]->GetPosition().y) % 2 == 1)
						_enemy[i]->SetPosition({ _enemy[i]->GetPosition().x, _enemy[i]->GetPosition().y + 1 });
				}
			}
		}
	}
	else
	{
		if (SOUNDMANAGER->IsPlaySound("intermission"))
			SOUNDMANAGER->Stop("intermission");
		for (int i = 0; i < 4; i++)
			if (_enemy[i]->GetState() == Enemy::State::Ghost)
				_enemy[i]->SetState(Enemy::State::Follow);
	}
}

void Stage::Render()
{
	//������Ʈ�� ������ �ִ� Ÿ���� ����, F1Ű�� ������ ��� Ÿ���� ����
	for (int i = 0; i < TILENUMY; i++)
	{
		for (int j = 0; j < TILENUMX; j++)
		{
			if (_tiles[i][j].haveObject)
			{
				_tileImage->SetSize(_tileImage->GetFrameSize());
				_tileImage->FrameRender(_tiles[i][j].rc.left, _tiles[i][j].rc.top, _tiles[i][j].objFrameX, _tiles[i][j].objFrameY, Pivot::LEFT_TOP);
			}
			if (KEYMANAGER->IsToggleKey(VK_F1))
			{
				D2DRENDERER->DrawRectangle(_tiles[i][j].rc, D2DRenderer::DefaultBrush::Green);
			}
			
		}
	}
	//Ÿ�� ���� ������Ʈ�� ����
	OBJECTMANAGER->Render();

	//UI�� ����
	CurrentScoreRender();
	HighScoreRender();
	KeySettingRender();
	if (_isStart)
	{
		bool b = false;
		for (int i = 0; i < 4; i++)
		{
			if (_enemy[i]->GetState() == Enemy::State::Confined)
				b = true;
		}
		if (b)
		{
			IMAGEMANAGER->FindImage("number")->SetSize({ 12, 12 });
			IMAGEMANAGER->FindImage("number")->FrameRender(TILENUMX / 2 * 12, _tiles[17][0].rc.top, 9 - (int)_addEnemyCount, 0);
		}
	}

	RenderRankingInterface();
}
//�Ű������� ���� ������Ʈ�κ��� �Ű������� ���� ������ Ÿ���� ��ȯ�ϴ� �Լ�
tagTile Stage::TileInDirection(GameObject* object, Direction::Enum direction)
{
	Vector2 position = object->GetPosition();
	INDEX objectIndex = { (int)((position.x - _tiles[0][0].rc.left) / TILESIZE), (int)((position.y - _tiles[0][0].rc.top) / TILESIZE) };
	tagTile tile = _tiles[objectIndex.y][objectIndex.x];
	switch (direction)
	{
	case Direction::Right:
		tile = _tiles[objectIndex.y][objectIndex.x + 1];
		break;
	case Direction::Down:
		tile = _tiles[objectIndex.y + 1][objectIndex.x];
		break;
	case Direction::Left:
		tile = _tiles[objectIndex.y][objectIndex.x - 1];
		break;
	case Direction::Up:
		tile = _tiles[objectIndex.y - 1][objectIndex.x];
		break;
	default:
		break;
	}
	return tile;
}

//�Ű������� ���� ������Ʈ�� Ÿ�ϰ� �浹�ϴ��� üũ�ϰ� �������ϴ� �������� �� �� �ִ��� üũ�ϴ� �Լ�
void Stage::CollisionOfGameObjectsWithTiles(GameObject* object)
{
	//������Ʈ�� �� �浹 �� - ������Ʈ�� ���� �����ϴ� Ÿ���� �ε����� ���� �ֺ� 9�� Ÿ�ϸ� �˻�
	POINT position = object->GetPosition();
	INDEX objectIndex = { (int)((position.x - _tiles[0][0].rc.left) / TILESIZE), (int)((position.y - _tiles[0][0].rc.top) / TILESIZE) };


	for (int i = objectIndex.y - 1; i <= objectIndex.y + 1; i++)
	{
		for (int j = objectIndex.x - 1; j <= objectIndex.x + 1; j++)
		{
			if (_tiles[i][j].haveObject)
			{
				object->MainRectCollision(_tiles[i][j].rc);
			}
		}
	}

	//������Ʈ�� �̵��� �� �ִ��� �Ǵ� ���� - ������Ʈ�� ���� �����ϴ� Ÿ���� �ε����� ���� ���ϴ� ������ Ÿ���� ������ Ȯ��
	Direction::Enum intendedDirection = object->GetIntendedDirection();
	Direction::Enum direction = object->GetDirection();
	tagTile tile = TileInDirection(object, intendedDirection);
	position = object->GetPosition();
	objectIndex = { (int)((position.x - _tiles[0][0].rc.left) / TILESIZE), (int)((position.y - _tiles[0][0].rc.top) / TILESIZE) };
	switch (intendedDirection)
	{
	case Direction::Right:
		object->SetIsChangeDirection(!tile.haveObject &&			//�ش� Ÿ���� ������ Ȯ��
			position.y == (tile.rc.top + tile.rc.bottom) / 2 &&		//�ش� Ÿ�Ͽ� �� �� �ִ� �������� Ȯ��
			direction != intendedDirection);						//�����̴� ����� �ٲٷ��� ������ �ٸ� �������� Ȯ��
		break;
	case Direction::Down:
		object->SetIsChangeDirection(!tile.haveObject &&
			position.x == (tile.rc.left + tile.rc.right) / 2 &&
			direction != intendedDirection);
		break;
	case Direction::Left:
		object->SetIsChangeDirection(!tile.haveObject &&
			position.y == (tile.rc.top + tile.rc.bottom) / 2 &&
			direction != intendedDirection);
		break;
	case Direction::Up:
		object->SetIsChangeDirection(!tile.haveObject &&
			position.x == (tile.rc.left + tile.rc.right) / 2 &&
			direction != intendedDirection);
		break;
	default:
		break;
	}
}
//�÷��̾�� ������, ���ʹ� �浹���� �ڵ�
void Stage::CollisionOfGamebjectWithPlayer(void)
{
	//�÷��̾�� �������� �浹�Ѵٸ� ���ھ �ش� ������ ������ �߰��ϰ� ������Ative�� ����. ���� BigBall�̸� ���ʹ̵� ��Ʈ ����
	for (int i = 0; i < _vItem.size(); i++)
	{
		if (!_vItem[i]->GetActive()) continue;
		if (Figure::IsCollision(_player->GetMainRect(), _vItem[i]->GetMainRect()))
		{
			if (!SOUNDMANAGER->IsPlaySound("eatfruit"))
				SOUNDMANAGER->Play("eatfruit");
			_score += _vItem[i]->GetPoint();
			_vItem[i]->SetActive(false);
			if (_vItem[i]->GetKind() == Item::Kind::BigBall)
			{
				if (SOUNDMANAGER->IsPlaySound("intermission"))
					SOUNDMANAGER->Stop("intermission");
				SOUNDMANAGER->Play("intermission");
				_isGhost = true;
				_ghostCount = 0;
			}
		}
	}

	//���ʹ̿� �÷��̾ �浹�ϸ� �÷��̾��� ���¸� �������� �ٲٰ� ���۰� ���� ������ �ٲ��ش�.
	for (int i = 0; i < 4; i++)
	{
		if (Figure::IsCollision(_player->GetMainRect(), _enemy[i]->GetMainRect()))
		{
			if (_vRanking.size() < 10)
			{
				_isShow = true;
			}
			else
			{
				if (_score > _vRanking[9].score)
					_isShow = true;
			}
			_isEnd = true;
			_isStart = false;
			_player->SetState(Player::State::Die);
			SOUNDMANAGER->Play("death");
			if (SOUNDMANAGER->IsPlaySound("intermission"))
				SOUNDMANAGER->Stop("intermission");
		}
		_enemy[i]->SetPlayerPos(_player->GetPosition());
	}
}
//�����ִ� ���ʹ̸� �߰��ϴ� �Լ�
void Stage::AddEnemy(Enemy* enemy)
{
	if (enemy->GetState() == Enemy::State::Confined)
	{
		enemy->SetPosition({ TILENUMX / 2 * 12, _tiles[14][0].rc.top + 6 });
		if (_isGhost)
			enemy->SetState(Enemy::State::Ghost);
		else
			enemy->SetState(Enemy::State::Follow);
	}
}
//���� ���� Ű ����
void Stage::KeySetting(void)
{
	if (_isShow)
	{
		if (KEYMANAGER->IsOnceKeyDown(VK_LEFT))
		{
			_currentInitial--;
			if (_currentInitial < 0)
			{
				_currentInitial = 2;
			}
		}
		else if (KEYMANAGER->IsOnceKeyDown(VK_RIGHT))
		{
			_currentInitial++;
			if (_currentInitial > 2)
			{
				_currentInitial = 0;
			}
		}
		else if (KEYMANAGER->IsOnceKeyDown(VK_UP))
		{
			_initialNum[_currentInitial]++;
			if (_initialNum[_currentInitial] > 25)
				_initialNum[_currentInitial] = 0;
		}
		else if (KEYMANAGER->IsOnceKeyDown(VK_DOWN))
		{
			_initialNum[_currentInitial]--;
			if (_initialNum[_currentInitial] < 0)
				_initialNum[_currentInitial] = 25;
		}
		if (KEYMANAGER->IsOnceKeyDown(VK_RETURN))
		{
			if (!_isRank)
				RecordInitial();
			else
			{
				_isShow = false;
			}
		}
	}
	if (_zoom > 0.0f) return;
	//�����̽��� ������ �� ���� �ֱ�
	if (KEYMANAGER->IsOnceKeyDown(VK_SPACE))
	{
		if (_isEnd)
		{
			ReStart();
		}
		else
		{
			if (SOUNDMANAGER->IsPlaySound("beginning"))
				SOUNDMANAGER->Stop("beginning");
			if (!_isStart)
			{
				_player->SetState(Player::State::Run);
				_isStart = true;
			}
			else
			{
				ReStart();
			}
		}
	}
}
//���� ���ھ� ����
void Stage::CurrentScoreRender()
{
	//���ĺ� �빮�� 65����
	AlphabetRender(_vvTileInfo[2][10].center.x, _vvTileInfo[2][10].center.y, 'S');
	AlphabetRender(_vvTileInfo[2][11].center.x, _vvTileInfo[2][11].center.y, 'C');
	AlphabetRender(_vvTileInfo[2][12].center.x, _vvTileInfo[2][12].center.y, 'O');
	AlphabetRender(_vvTileInfo[2][13].center.x, _vvTileInfo[2][13].center.y, 'R');
	AlphabetRender(_vvTileInfo[2][14].center.x, _vvTileInfo[2][14].center.y, 'E');

	ScoreNumRender(_score, _vvTileInfo[2][16].center.x, _vvTileInfo[2][16].center.y);
}

//�ְ� ���ھ� ����
void Stage::HighScoreRender()
{
	AlphabetRender(_vvTileInfo[0][5].center.x, _vvTileInfo[0][5].center.y, 'H');
	AlphabetRender(_vvTileInfo[0][6].center.x, _vvTileInfo[0][6].center.y, 'I');
	AlphabetRender(_vvTileInfo[0][7].center.x, _vvTileInfo[0][7].center.y, 'G');
	AlphabetRender(_vvTileInfo[0][8].center.x, _vvTileInfo[0][8].center.y, 'H');
	AlphabetRender(_vvTileInfo[0][10].center.x, _vvTileInfo[0][10].center.y, 'S');
	AlphabetRender(_vvTileInfo[0][11].center.x, _vvTileInfo[0][11].center.y, 'C');
	AlphabetRender(_vvTileInfo[0][12].center.x, _vvTileInfo[0][12].center.y, 'O');
	AlphabetRender(_vvTileInfo[0][13].center.x, _vvTileInfo[0][13].center.y, 'R');
	AlphabetRender(_vvTileInfo[0][14].center.x, _vvTileInfo[0][14].center.y, 'E');

	if (_vRanking.size() == 0) return;

	//16~18
	for(int i = 0; i < 3; i++)
		AlphabetRender(_vvTileInfo[0][16 + i].center.x, _vvTileInfo[0][16 + i].center.y, _vRanking[0].initial[i]);

	vector<int> vScore;
	int score = _vRanking[0].score;
	while (score > 0)
	{
		vScore.push_back(score % 10);
		score /= 10;
	}
	if (_vRanking[0].score == 0)
	{
		vScore.push_back(0);
	}
	int size = vScore.size();
	for (int i = size - 1; i >= 0; i--)
	{
		_numberImage->SetSize({ 12, 12 });
		_numberImage->FrameRender(_vvTileInfo[0][20].center.x + ((size - i) - 1) * 12, _vvTileInfo[0][20].center.y, vScore[i], 0);
	}
}
//�����̳� �����⸦ �Ϸ��� � Ű�� �������ϴ��� ������ �������� �ϴܿ� ����
void Stage::KeySettingRender()
{
	//RESTART PRESS SPACE
	AlphabetRender(_vvTileInfo[34][5].center.x, _vvTileInfo[34][5].center.y, 'R');
	AlphabetRender(_vvTileInfo[34][6].center.x, _vvTileInfo[34][6].center.y, 'E');
	AlphabetRender(_vvTileInfo[34][7].center.x, _vvTileInfo[34][7].center.y, 'S');
	AlphabetRender(_vvTileInfo[34][8].center.x, _vvTileInfo[34][8].center.y, 'T');
	AlphabetRender(_vvTileInfo[34][9].center.x, _vvTileInfo[34][9].center.y, 'A');
	AlphabetRender(_vvTileInfo[34][10].center.x, _vvTileInfo[34][10].center.y, 'R');
	AlphabetRender(_vvTileInfo[34][11].center.x, _vvTileInfo[34][11].center.y, 'T');

	AlphabetRender(_vvTileInfo[34][13].center.x, _vvTileInfo[34][13].center.y, 'P');
	AlphabetRender(_vvTileInfo[34][14].center.x, _vvTileInfo[34][14].center.y, 'R');
	AlphabetRender(_vvTileInfo[34][15].center.x, _vvTileInfo[34][15].center.y, 'E');
	AlphabetRender(_vvTileInfo[34][16].center.x, _vvTileInfo[34][16].center.y, 'S');
	AlphabetRender(_vvTileInfo[34][17].center.x, _vvTileInfo[34][17].center.y, 'S');

	AlphabetRender(_vvTileInfo[34][19].center.x, _vvTileInfo[34][19].center.y, 'S');
	AlphabetRender(_vvTileInfo[34][20].center.x, _vvTileInfo[34][20].center.y, 'P');
	AlphabetRender(_vvTileInfo[34][21].center.x, _vvTileInfo[34][21].center.y, 'A');
	AlphabetRender(_vvTileInfo[34][22].center.x, _vvTileInfo[34][22].center.y, 'C');
	AlphabetRender(_vvTileInfo[34][23].center.x, _vvTileInfo[34][23].center.y, 'E');

	//EXIT PRESS ESC
	AlphabetRender(_vvTileInfo[35][5].center.x, _vvTileInfo[35][5].center.y, 'E');
	AlphabetRender(_vvTileInfo[35][6].center.x, _vvTileInfo[35][6].center.y, 'X');
	AlphabetRender(_vvTileInfo[35][7].center.x, _vvTileInfo[35][7].center.y, 'I');
	AlphabetRender(_vvTileInfo[35][8].center.x, _vvTileInfo[35][8].center.y, 'T');

	AlphabetRender(_vvTileInfo[35][13].center.x, _vvTileInfo[35][13].center.y, 'P');
	AlphabetRender(_vvTileInfo[35][14].center.x, _vvTileInfo[35][14].center.y, 'R');
	AlphabetRender(_vvTileInfo[35][15].center.x, _vvTileInfo[35][15].center.y, 'E');
	AlphabetRender(_vvTileInfo[35][16].center.x, _vvTileInfo[35][16].center.y, 'S');
	AlphabetRender(_vvTileInfo[35][17].center.x, _vvTileInfo[35][17].center.y, 'S');

	AlphabetRender(_vvTileInfo[35][19].center.x, _vvTileInfo[35][19].center.y, 'E');
	AlphabetRender(_vvTileInfo[35][20].center.x, _vvTileInfo[35][20].center.y, 'S');
	AlphabetRender(_vvTileInfo[35][21].center.x, _vvTileInfo[35][21].center.y, 'C');
}

//�ٽý��� �Լ�(�ٽ� ������ �� �ʱ�ȭ �ؾ��ϴ� ������ ������Ʈ ��ġ �� active �ʱ�ȭ)
void Stage::ReStart()
{
	SOUNDMANAGER->Play("beginning");
	_isStart = false;
	_isEnd = false;
	_ghostCount = 0.0f;
	_addEnemyCount = 0.0f;
	_isGhost = false;
	_score = 0;
	for (int i = 0; i < 3; i++)
		_initialNum[i] = 0;
	_currentInitial = 0;
	_player->SetActive(true);
	_player->SetPosition({ TILENUMX / 2 * 12, _tiles[26][0].rc.top + 6 });
	_enemy[0]->SetPosition({ TILENUMX / 2 * 12, _tiles[14][0].rc.top + 6 });
	_enemy[1]->SetPosition({ TILENUMX / 2 * 12 - 24, _tiles[17][0].rc.top + 6 });
	_enemy[2]->SetPosition({ TILENUMX / 2 * 12, _tiles[17][0].rc.top + 6 });
	_enemy[3]->SetPosition({ TILENUMX / 2 * 12 + 24, _tiles[17][0].rc.top + 6 });
	for (int i = 0; i < 4; i++)
	{
		if(i == 0)
			_enemy[i]->SetState(Enemy::State::Follow);
		else
			_enemy[i]->SetState(Enemy::State::Confined);
		_enemy[i]->SetPlayerPos(_player->GetPosition());
	}
	for (int i = 0; i < OBJECTMANAGER->GetObjectList(ObjectType::UpObject).size(); i++)
	{
		OBJECTMANAGER->GetObjectList(ObjectType::UpObject)[i]->Init();
	}
	for (int i = 0; i < _vItem.size(); i++)
	{
		_vItem[i]->ResetItem();
		_vItem[i]->SetActive(true);
	}
}
//�̴ϼ� ���
void Stage::RecordInitial()
{
	tagRanking ranking;
	for (int i = 0; i < 3; i++)
	{
		ranking.initial.push_back((char)(_initialNum[i] + 65));
	}
	ranking.score = _score;

	int num;
	if (_vRanking.size() < 10)
	{
		_vRanking.push_back(ranking);
		num = _vRanking.size() - 1;
	}
	else
	{
		num = 9;
		_vRanking[num] = ranking;
	}
	
	bool find = false;
	while (num >= 1 && _vRanking[num - 1].score < _vRanking[num].score)
	{
		tagRanking temp = _vRanking[num - 1];
		_vRanking[num - 1] = _vRanking[num];
		_vRanking[num] = temp;
		num--;
	}
	_isRank = true;
	_interface.maxSize = { 320, 250 };
	SaveRanking();
}
//�ְ� ���� ����
void Stage::SaveRanking()
{
	ofstream outFile("./Ranking.txt");
	for (int i = 0; i < _vRanking.size(); i++)
	{
		outFile << _vRanking[i].initial << _vRanking[i].score << ';' << endl;
	}
	outFile.close();
}
//�ְ� ���� �ҷ�����
void Stage::LoadRanking()
{
	//���� �о���̱�
	ifstream inFile("./Ranking.txt");
	if (!inFile.is_open()) return;

	string str;
	while (!inFile.eof())
	{
		getline(inFile, str);
		if (str == "") break;
		tagRanking ranking;
		ranking.initial = "";
		ranking.score = 0;
		for (int i = 0; i < 3; i++)
		{
			ranking.initial.push_back(str[i]);
		}
		for (int i = 3; i < str.size(); i++)
		{
			if (str[i] == ';')
				break;
			ranking.score += ((((double)str[i] - 48) * pow(10, (str.size() - (i + 2)))));
		}

		_vRanking.push_back(ranking);
	}
	inFile.close();

	//�о���� ������ �����Ͱ� 2�� �̻��̸� ����
	if (_vRanking.size() < 2) return;
	//��������
	tagRanking temp;
	int maxNum, index = 0;
	int arrSize = _vRanking.size();
	for (int i = 0; i < arrSize; i++)
	{
		//������ 0�� �ε������� �Ұ��̹Ƿ� �ι�° for���� i��°���� �����Ѵ�.
		for (int j = i; j < arrSize; j++)
		{
			//�˻� ������ �� �ּڰ��� �ε����� �˻��ϴ� ù��° ���ڷ� �����ش�.
			if (j == i)
			{
				maxNum = _vRanking[j].score;
				index = j;
			}
			//�˻� ���߿� �ּڰ����� �۰ų� ���� ���ڰ� �߰ߵǸ� �ε����� �ּڰ��� �ٲ��ش�.
			if (_vRanking[j].score >= maxNum)
			{
				maxNum = _vRanking[j].score;
				index = j;
			}
		}
		//�˻簡 ������ �ּڰ��� ����ִ� �ε����� ���ĵ� �� ���� �ε����� ��������
		temp = _vRanking[i];
		_vRanking[i] = _vRanking[index];
		_vRanking[index] = temp;
	}
}
//��ŷ �������̽� ������Ʈ
void Stage::UpdateRankingInterface()
{
	//UI ��Ÿ���� ������� �ڵ�
	if (_isShow)
	{
		_zoom += 0.03f;
		if (_zoom >= 1.0f)
			_zoom = 1.0f;
	}
	else
	{
		_zoom -= 0.03f;
		if (_zoom <= 0.0f)
		{
			_zoom = 0.0f;
			_isRank = false;
			_interface.maxSize = { 320, 150 };
		}
	}
	_interface.size = { (long)(_interface.maxSize.x * _zoom), (long)(_interface.maxSize.y * _zoom) };
	_interface.rc = Figure::RectMakeCenter(_interface.pos.x, _interface.pos.y, _interface.size.x, _interface.size.y);
	for (int i = 0; i < 3; i++)
	{
		_initial[i].pos = { _interface.pos.x + (int)((i - 1) * 50 * _zoom), _interface.pos.y + (int)(30 * _zoom) };
		_initial[i].size = { (long)(_initial[i].maxSize.x * _zoom), (long)(_initial[i].maxSize.y * _zoom) };
		_initial[i].rc = Figure::RectMakeCenter(_initial[i].pos.x, _initial[i].pos.y, _initial[i].size.x, _initial[i].size.y);
	}
}
//��ŷ �������̽� ����
void Stage::RenderRankingInterface()
{
	IMAGEMANAGER->FindImage("recordInterface")->SetSize({ (float)_interface.size.x, (float)_interface.size.y });
	IMAGEMANAGER->FindImage("recordInterface")->Render((float)_interface.pos.x, (float)_interface.pos.y);
	if (!_isRank)
	{
		for (int i = 0; i < 3; i++)
		{
			AlphabetRender(_initial[i].pos.x - (4.0f * _zoom), _initial[i].pos.y + (4.0f * _zoom), (char)(_initialNum[i] + 65), { (float)_initial[i].size.x, (float)_initial[i].size.y });
			if (_currentInitial != i) continue;
			IMAGEMANAGER->FindImage("currentInitial")->SetSize({ (float)_initial[i].size.x, (float)_initial[i].size.y });
			IMAGEMANAGER->FindImage("currentInitial")->Render(_initial[i].pos.x, _initial[i].pos.y);
		}
		AlphabetRender(_interface.pos.x - (24 * _zoom), _interface.pos.y - (48 * _zoom), 'S', { 12.0f * _zoom, 12.0f * _zoom });
		AlphabetRender(_interface.pos.x - (12 * _zoom), _interface.pos.y - (48 * _zoom), 'C', { 12.0f * _zoom, 12.0f * _zoom });
		AlphabetRender(_interface.pos.x, _interface.pos.y - (48 * _zoom), 'O', { 12.0f * _zoom, 12.0f * _zoom });
		AlphabetRender(_interface.pos.x + (12 * _zoom), _interface.pos.y - (48 * _zoom), 'R', { 12.0f * _zoom, 12.0f * _zoom });
		AlphabetRender(_interface.pos.x + (24 * _zoom), _interface.pos.y - (48 * _zoom), 'E', { 12.0f * _zoom, 12.0f * _zoom });

		ScoreNumRender(_score, _interface.pos.x, _interface.pos.y - (24.0f * _zoom), 12.0f * _zoom, Pivot::CENTER);
	}
	else
	{
		for (int i = 0; i < _vRanking.size(); i++)
		{
			IMAGEMANAGER->FindImage("number")->SetSize({ 12.0f * _zoom, 12.0f * _zoom });
			if (i == 9)
			{
				IMAGEMANAGER->FindImage("number")->FrameRender(_interface.rc.left + (18 * _zoom), _interface.rc.top + (24 + 20 * i) * _zoom, 1, 0);
				IMAGEMANAGER->FindImage("number")->SetSize({ 12.0f * _zoom, 12.0f * _zoom });
				IMAGEMANAGER->FindImage("number")->FrameRender(_interface.rc.left + (30 * _zoom), _interface.rc.top + (24 + 20 * i) * _zoom, 0, 0);
			}
			else
				IMAGEMANAGER->FindImage("number")->FrameRender(_interface.rc.left + (24 * _zoom), _interface.rc.top + (24 + 20 * i) * _zoom, i + 1, 0);
			for(int j = 0; j < 3; j++)
				AlphabetRender(_interface.rc.left + (48 + 12 * j) * _zoom, _interface.rc.top + (24 + i * 20) * _zoom, _vRanking[i].initial[j], {12.0f * _zoom, 12.0f * _zoom});
			ScoreNumRender(_vRanking[i].score, _interface.rc.left + (92 * _zoom), _interface.rc.top + (24 + i * 20) * _zoom, 12.0f * _zoom);
		}
	}
}


/*********************************************************
	## ���ĺ� ���� �Լ� ##
	@@ float x : ������ X ��ǥ
	@@ float y : ������ Y ��ǥ
	@@ char c : ������ ����
	@@ POINTFLOAT size : ������ ������
**********************************************************/
void Stage::AlphabetRender(float x, float y, char c, POINTFLOAT size)
{
	//���ĺ� �빮�� 65����
	_alphabetImage->SetSize({ size.x, size.y });
	_alphabetImage->FrameRender(x, y, (int)c - 65, 0);
}
/*********************************************************
	## ���� ���� �Լ� ##
	@@ int number : ������ ���ϴ� ����
	@@ int startX : ���� ���� X ��ǥ
	@@ int startY : ���� ���� Y ��ǥ
	@@ float size : ������ ������
	@@ Pivot::Enum pivot : ������ ���ϴ� �Ǻ�(���� or ���)
**********************************************************/
void Stage::ScoreNumRender(int number, int startX, int startY, float size, Pivot::Enum pivot)
{
	vector<int> vScore;
	int score = number;
	while (score > 0)
	{
		vScore.push_back(score % 10);
		score /= 10;
	}
	if (_score == 0)
	{
		vScore.push_back(0);
	}
	int scoreSize = vScore.size();
	for (int i = scoreSize - 1; i >= 0; i--)
	{
		_numberImage->SetSize({ size, size });
		switch (pivot)
		{
		case Pivot::LEFT_TOP:	//�̰Ŵ� left_top�� �ƴϰ� �׳� ���ʺ��� ������ �ǹ�
			_numberImage->FrameRender(startX + ((scoreSize - i) - 1) * size, startY, vScore[i], 0);
			break;
		case Pivot::CENTER:		//�̰Ŵ� ��� ������ �ǹ�
			_numberImage->FrameRender((float)startX + (size / 2.0f) + ((((float)scoreSize / 2.0f) - (float)(i + 1)) * size), startY, vScore[i], 0);
			break;
		default:
			_numberImage->FrameRender(startX + ((scoreSize - i) - 1) * size, startY, vScore[i], 0);
			break;
		}
	}
}
