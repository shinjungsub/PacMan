#pragma once

class AStar
{
private:
	vector<vector<tagAstar*>> _vTotalList;		//��ü Ÿ�� ������ ��� �ִ� ����
	vector<tagAstar*> _vOpenList;				//���¸���Ʈ
	vector<Direction::Enum> _vDirectionList;	//���� �� ã�� �� ���� ��� �������� �������� �ϴ����� ��� �ִ� ����

	POINT _startIndex;	//A*�� ����ϴ� ������Ʈ�� ���� �ε���
	POINT _endIndex;	//ã�ư����� �ϴ� �ε���

	bool _isStart;
public:
	void SetTotalList(vector<vector<tagAstar>> tiles);
	void SetTiles(POINT start, POINT end);
	vector<tagAstar*> AddOpenList(POINT currentIndex);
	void PathFinder(POINT currentIndex);
	Direction::Enum ReturnDirection(POINT currentIndex, POINT parentIndex);
	Direction::Enum GetDirection()
	{
		if (_vDirectionList.size() == 0) return Direction::Down;
		Direction::Enum direct = _vDirectionList[_vDirectionList.size() - 1];
		_vDirectionList.pop_back();
		return direct;
	}
	AStar() {}
	~AStar()
	{
		_vOpenList.clear();
		_vDirectionList.clear();
		for(int i = 0; i < _vTotalList.size(); i++)
			for(int j = 0; j < _vTotalList[i].size(); j++)
				SAFE_DELETE(_vTotalList[i][j]);
		_vTotalList.clear();
	}
};

