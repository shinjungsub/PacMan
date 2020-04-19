#include "stdafx.h"
#include "AStar.h"

//���� ���� ���Ϳ� Ÿ�� ������ �޾ƿ�(���� ������ ���ʹ̵��� ���� �ּҸ� �ǵ�� �ȵǱ� ������)
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
//������ �ʱ�ȭ(�ʱ� �˻��� ��˻��� ���)
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
//���� �ε��� �ֺ� Ÿ���� ���¸���Ʈ�� �߰�
vector<tagAstar*> AStar::AddOpenList(POINT currentIndex)
{
	tagAstar* currentTile = _vTotalList[currentIndex.y][currentIndex.x];
	int startX = currentIndex.x - 1;
	int startY = currentIndex.y - 1;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			//�˻� ���
			tagAstar* node = _vTotalList[startY + i][startX + j];
			//��ŸƮ �ε����� ������ continue
			//���� �ε����� ������ continue
			//���� �ε������� �밢���� �ִ� �ε����̸� continue -> �Ѹ��� �밢������ �̵����� �ʱ� ����
			//��尡 object�� ������ ������ continue
			//��尡 close�̸� continue
			if (Figure::IsEqualPoint(node->index, _startIndex)) continue;
			if (Figure::IsEqualPoint(node->index, currentIndex)) continue;
			if (abs(node->index.x - currentIndex.x) == 1 && abs(node->index.y - currentIndex.y) == 1) continue;
			if (node->haveObject) continue;
			if (node->isClose) continue;
			//��尡 ���µǾ� ���� ��� ����� �θ����� G���� �θ�� ������ �����̾��� ����� ���� ����� G����� ���ؼ�
			//���� ����� G����� �� ���� ���
			//�ش� ����� �θ� ��带 ���� ���� �ٲٰ� G���� F��뵵 �׿� ���� �ٲ���
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

			//�� ���� �ܰ���� ��� ��������� ok �� ��� �ϸ鼭 �θ� ��带 ���� ���� �����ְ� ���¸���Ʈ�� �־���
			node->parentTile = currentTile;
			_vOpenList.push_back(node);
		}
	}
	return _vOpenList;
}

//��ã�� �Լ�
void AStar::PathFinder(POINT currentIndex)
{
	//���� ������ �ε����� ������ ������!!
	if (Figure::IsEqualPoint(currentIndex, _endIndex)) return;
	//ó�� ���� F���� �۰� ������ ���� �� ���� �׷��� Ȯ���ϰ� ���� ���ڷ� ���ؾ���
	float tempTotalCost = 100000.0f;
	//�̳��� ���ϸ鼭 ������ �������� �ٲ��� ����
	tagAstar* tempTile = nullptr;

	//�ϴ� ���� �ε����� �������� ���¸���Ʈ�� �߰�
	int openListSize = AddOpenList(currentIndex).size();
	//�߰��� ���¸���Ʈ�� ���� �˻� ����
	for (int i = 0; i < openListSize; i++)
	{
		//���� H�� �����ֱ� - ����ź������� ���(�������� ���ϴ� ���� ���� �����ϰ� ������ �� �������� ���μ��� 10������ ���)
		_vOpenList[i]->costToEnd = (abs(_endIndex.x - _vOpenList[i]->index.x) + abs(_endIndex.y - _vOpenList[i]->index.y)) * 10;
		//���� G�� �����ֱ� - �θ����� G���� 10�� ����(�Ѹ��� �밢�������� �̵��� ���� ������ �׳� 10�� ���ϸ� ��)
		//���� �밢�� �̵��� �ִٸ� �ε����� ���Ͽ� �밢������ �ƴ��� üũ �� 10�̳� 14�� �θ� ����� G���� �����ָ� ��
		_vOpenList[i]->costFromStart = 10 + (_vOpenList[i]->parentTile == nullptr ? 0 : _vOpenList[i]->parentTile->costFromStart);
		//���� F�� �����ֱ� - F = G + H
		_vOpenList[i]->totalCost = _vOpenList[i]->costToEnd + _vOpenList[i]->costFromStart;
		//�˻��� ����� F���� tempF�� ���� �۴ٸ� �������ְ� tempTile�� ����
		if (tempTotalCost > _vOpenList[i]->totalCost)
		{
			tempTotalCost = _vOpenList[i]->totalCost;
			tempTile = _vOpenList[i];
		}
		//�˻��� ���� ����ôٴ°� �˷���� �ϹǷ� isOpen�� true�� ����
		_vOpenList[i]->isOpen = true;
	}

	//�ؿ��� tempTile�� ������ ������ �����Ƿ� ���� nullptr�̸� �׳� �Լ��� �������� ����ó��
	if (tempTile == nullptr) return;

	//���� tempTile�� �ε����� ������ �ε����� ���ٸ�
	//�θ� nullptr�� �ɶ����� Ÿ�� ���鼭 �̵��� ������ ���Ϳ� �־���
	//�׸��� return
 	if (Figure::IsEqualPoint(tempTile->index, _endIndex))
	{
		while (tempTile->parentTile != nullptr)
		{
			_vDirectionList.push_back(ReturnDirection(tempTile->index, tempTile->parentTile->index));
			tempTile = tempTile->parentTile;
		}
		return;
	}
	//������ �°��� ���� tempTile�� ������ �ε������� �����ٴ� ���
	//���� tempTile�� �̹� �˻簡 ���� ����̹Ƿ� ���̻� �˻��� �ʿ䰡 ���ٴ� isClose�� true�� �ٲ��ְ�
	//���¸���Ʈ���� ����� isOpen�� false�� �ٲ��� - �̰� ���� ��� ����
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
	//������ �ε����� ã�� ������ �ݺ�����~~~!!!
	PathFinder(tempTile->index);
}

//�ε����� ���ؼ� ��� �������� �����ϴ� ������ �˷��ִ� �Լ�
//parentIndex�� A*�� ����ϴ� ������Ʈ�� ���� ��ġ�� �� ����� ��
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
