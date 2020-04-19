#pragma once

class AStar
{
private:
	vector<vector<tagAstar*>> _vTotalList;		//전체 타일 정보를 담고 있는 벡터
	vector<tagAstar*> _vOpenList;				//오픈리스트
	vector<Direction::Enum> _vDirectionList;	//길을 다 찾고 난 이후 어느 방향으로 움직여야 하는지를 담고 있는 벡터

	POINT _startIndex;	//A*를 사용하는 오브젝트의 시작 인덱스
	POINT _endIndex;	//찾아가려고 하는 인덱스

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

