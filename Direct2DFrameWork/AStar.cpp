#include "stdafx.h"
#include "AStar.h"

//따로 만든 벡터에 타일 정보를 받아옴(여러 마리의 에너미들이 같은 주소를 건들면 안되기 때문에)
void AStar::SetTotalList(vector<vector<tagAstar>> tiles)
{
	if (_vTotalList.size() != 0) return;
	for (int i = 0; i < TILENUMY; i++)
	{
		vector<tagAstar*> vAstar;
		for (int j = 0; j < TILENUMX; j++)
		{
			tagAstar* aStar = new tagAstar;
			aStar->Init(j, i, tiles[i][j].haveObject);
			vAstar.push_back(aStar);
		}
		_vTotalList.push_back(vAstar);
	}
}
//정보들 초기화(초기 검색과 재검색시 사용)
void AStar::SetTiles(POINT start, POINT end)
{
	_vOpenList.clear();
	_vDirectionList.clear();

	for (int i = 0; i < TILENUMY; i++)
	{
		for (int j = 0; j < TILENUMX; j++)
		{
			_vTotalList[i][j]->costFromStart = 0;
			_vTotalList[i][j]->costToEnd = 0;
			_vTotalList[i][j]->totalCost = 0;
			_vTotalList[i][j]->isClose = false;
			_vTotalList[i][j]->isOpen = false;
			_vTotalList[i][j]->parentTile = nullptr;
		}
	}

	_startIndex = { (int)(start.x / TILESIZE), (int)((start.y - 120) / TILESIZE) };
	_endIndex = { (int)(end.x / TILESIZE) , (int)((end.y - 120) / TILESIZE) };
}
//현재 인덱스 주변 타일을 오픈리스트에 추가
vector<tagAstar*> AStar::AddOpenList(POINT currentIndex)
{
	tagAstar* currentTile = _vTotalList[currentIndex.y][currentIndex.x];
	int startX = currentIndex.x - 1;
	int startY = currentIndex.y - 1;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			//검색 노드
			tagAstar* node = _vTotalList[startY + i][startX + j];
			//스타트 인덱스와 같으면 continue
			//현재 인덱스와 같으면 continue
			//현재 인덱스에서 대각선에 있는 인덱스이면 continue -> 팩맨은 대각선으로 이동하지 않기 때문
			//노드가 object를 가지고 있으면 continue
			//노드가 close이면 continue
			if (Figure::IsEqualPoint(node->index, _startIndex)) continue;
			if (Figure::IsEqualPoint(node->index, currentIndex)) continue;
			if (abs(node->index.x - currentIndex.x) == 1 && abs(node->index.y - currentIndex.y) == 1) continue;
			if (node->haveObject) continue;
			if (node->isClose) continue;
			//노드가 오픈되어 있을 경우 노드의 부모노드의 G비용과 부모로 지정할 예정이었던 노드인 현재 노드의 G비용을 비교해서
			//현재 노드의 G비용이 더 작을 경우
			//해당 노드의 부모 노드를 현재 노드로 바꾸고 G비용과 F비용도 그에 따라 바꿔줌
			//continue
			if (node->isOpen)
			{
				if (node->parentTile == nullptr) continue;
				if (node->parentTile->costFromStart > currentTile->costFromStart)
				{
					node->parentTile = currentTile;
					node->costFromStart = 10 + node->parentTile->costFromStart;
					node->totalCost = (node->costToEnd + node->costFromStart);
				}
				continue;
			}

			//위 여러 단계들을 모두 통과했으면 ok 너 통과 하면서 부모 노드를 현재 노드로 정해주고 오픈리스트에 넣어줌
			node->parentTile = currentTile;
			_vOpenList.push_back(node);
		}
	}
	return _vOpenList;
}

//길찾기 함수
void AStar::PathFinder(POINT currentIndex)
{
	//만약 마지막 인덱스와 같으면 나가라!!
	if (Figure::IsEqualPoint(currentIndex, _endIndex)) return;
	//처음 비교할 F값을 작게 잡으면 꼬일 수 있음 그래서 확실하게 높은 숫자로 정해야함
	float tempTotalCost = 100000.0f;
	//이놈은 비교하면서 노드들의 정보들을 바꿔줄 변수
	tagAstar* tempTile = nullptr;

	//일단 현재 인덱스를 기준으로 오픈리스트에 추가
	int openListSize = AddOpenList(currentIndex).size();
	//추가된 오픈리스트를 토대로 검사 시작
	for (int i = 0; i < openListSize; i++)
	{
		//노드들 H값 정해주기 - 맨하탄방법으로 계산(지나가지 못하는 곳들 전부 무시하고 노드부터 끝 지점까지 가로세로 10씩으로 계산)
		_vOpenList[i]->costToEnd = (abs(_endIndex.x - _vOpenList[i]->index.x) + abs(_endIndex.y - _vOpenList[i]->index.y)) * 10;
		//노드들 G값 정해주기 - 부모노드의 G값에 10을 더함(팩맨은 대각선으로의 이동이 없기 때문에 그냥 10만 더하면 됨)
		//만약 대각선 이동이 있다면 인덱스를 비교하여 대각선인지 아닌지 체크 후 10이나 14를 부모 노드의 G값에 더해주면 됨
		_vOpenList[i]->costFromStart = 10 + (_vOpenList[i]->parentTile == nullptr ? 0 : _vOpenList[i]->parentTile->costFromStart);
		//노드들 F값 정해주기 - F = G + H
		_vOpenList[i]->totalCost = _vOpenList[i]->costToEnd + _vOpenList[i]->costFromStart;
		//검색한 노드의 F값이 tempF값 보다 작다면 대입해주고 tempTile도 변경
		if (tempTotalCost > _vOpenList[i]->totalCost)
		{
			tempTotalCost = _vOpenList[i]->totalCost;
			tempTile = _vOpenList[i];
		}
		//검색한 노드는 열어봤다는걸 알려줘야 하므로 isOpen을 true로 변경
		_vOpenList[i]->isOpen = true;
	}

	//밑에는 tempTile에 직접적 연관이 있으므로 만약 nullptr이면 그냥 함수를 나오도록 예외처리
	if (tempTile == nullptr) return;

	//만약 tempTile의 인덱스가 마지막 인덱스와 같다면
	//부모가 nullptr이 될때까지 타고 가면서 이동할 방향을 벡터에 넣어줌
	//그리고 return
 	if (Figure::IsEqualPoint(tempTile->index, _endIndex))
	{
		while (tempTile->parentTile != nullptr)
		{
			_vDirectionList.push_back(ReturnDirection(tempTile->index, tempTile->parentTile->index));
			tempTile = tempTile->parentTile;
		}
		return;
	}
	//요기까지 온것은 아직 tempTile이 마지막 인덱스까지 못갔다는 얘기
	//따라서 tempTile은 이미 검사가 끝난 노드이므로 더이상 검사할 필요가 없다는 isClose를 true로 바꿔주고
	//오픈리스트에서 지우고 isOpen도 false로 바꿔줌 - 이건 별로 상관 없음
	tempTile->isClose = true;
	int size = _vOpenList.size();
	for (int i = 0; i < size; i++)
	{
		if (_vOpenList[i] == tempTile)
		{
			_vOpenList.erase(_vOpenList.begin() + i);
			tempTile->isOpen = false;
			break;
		}
	}
	//마지막 인덱스를 찾을 때까지 반복하자~~~!!!
	PathFinder(tempTile->index);
}

//인덱스를 비교해서 어느 방향으로 가야하는 것인지 알려주는 함수
//parentIndex가 A*를 사용하는 오브젝트의 현재 위치에 더 가까운 것
Direction::Enum AStar::ReturnDirection(POINT currentIndex, POINT parentIndex)
{
	if ((Math::Abs)(parentIndex.x - currentIndex.x) == 1)
	{
		if (parentIndex.x > currentIndex.x)
		{
			return Direction::Left;
		}
		else
		{
			return Direction::Right;
		}
	}
	else if ((Math::Abs)(parentIndex.y - currentIndex.y) == 1)
	{
		if (parentIndex.y > currentIndex.y)
		{
			return Direction::Up;
		}
		else
		{
			return Direction::Down;
		}
	}
	return Direction::Enum();
}
