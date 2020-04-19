#include "stdafx.h"
#include "MapTool.h"


void MapTool::Init()
{
	//�̹��� �߰��ϱ�
	_tileImage = IMAGEMANAGER->FindImage("tile");

	//��������
	this->maptoolSetup();
	
	_sampleDragStart = {0, 0};
	_sampleDragEnd = {0, 0};
	_sampleDragNumX = 0;
	_sampleDragNumY = 0;

	_tileDragStart = { 0, 0 };
	_tileDragEnd = { 0, 0 };
	_tileDragNumX = 0;
	_tileDragNumY = 0;
	_isTileDrag = false;
	_isDragStart = false;

	_ctrlSelect = CTRL::END;
	_tileType = TILETYPE::ERASER;

	//���콺�� ī�޶� �ε��� �ʱ�ȭ
	_mouseIdx.x = _ptMouse.x / TILESIZE;
	_mouseIdx.y = (_ptMouse.y - 120.0f) / TILESIZE;
}

void MapTool::Release()
{
}

void MapTool::Update()
{
	//���콺�� ī�޶� �ε��� ������Ʈ
	_mouseIdx.x = _ptMouse.x / TILESIZE;
	_mouseIdx.y = (_ptMouse.y - 120.0f) / TILESIZE;

	if (KEYMANAGER->IsOnceKeyDown(VK_LBUTTON))
	{
		for (int i = 0; i < SAMPLETILEY; i++)
		{
			for (int j = 0; j < SAMPLETILEX; j++)
			{
				if (PtInRect(&_sampleTile[i][j].rc, _ptMouse))
				{
					_sampleDragStart.x = _sampleTile[i][j].frameX;
					_sampleDragStart.y = _sampleTile[i][j].frameY;
					break;
				}
			}
		}
		
		this->clickButton();
		this->tileDrag();
	}
	if (KEYMANAGER->IsOnceKeyUp(VK_LBUTTON))
	{
		_ctrlSelect = CTRL::END;
		if (_isTileDrag)
		{
			_isDragStart = true;
			_isTileDrag = false;
		}
	}
	if (KEYMANAGER->IsStayKeyDown(VK_LBUTTON))
	{
		for (int i = 0; i < SAMPLETILEY; i++)
		{
			for (int j = 0; j < SAMPLETILEX; j++)
			{
				if (PtInRect(&_sampleTile[i][j].rc, _ptMouse))
				{
					_sampleDragEnd.x = _sampleTile[i][j].frameX;
					_sampleDragEnd.y = _sampleTile[i][j].frameY;
					break;
				}
			}
		}

		this->setMap();
	}

	if (_isDragStart)
	{
		for (int i = _tileDragStart.y; i < _tileDragEnd.y + 1; i++)
		{
			for (int j = _tileDragStart.x; j < _tileDragEnd.x + 1; j++)
			{
				if (_tileType == TILETYPE::OBJECT)
				{
					_tiles[i][j].objFrameX = _sampleDragStart.x;
					_tiles[i][j].objFrameY = _sampleDragStart.y;
					_tiles[i][j].haveObject = true;
				}
				else if (_tileType == TILETYPE::ERASER)
				{
					_tiles[i][j].haveObject = false;
				}
			}
		}
		_isDragStart = false;
	}
	
	_sampleDragNumX = _sampleDragEnd.x - _sampleDragStart.x;
	_sampleDragNumY = _sampleDragEnd.y - _sampleDragStart.y;
}

void MapTool::Render()
{
	//Ÿ�� ��ġǥ���ϱ����� Ÿ�Ϸ���
	if (KEYMANAGER->IsToggleKey(VK_F1))
	{
		for (int i = 0; i < TILENUMY; i++)
		{
			for (int j = 0; j < TILENUMX; j++)
			{
				D2DRENDERER->DrawRectangle(_tiles[i][j].rc, D2DRenderer::DefaultBrush::Gray);
			}
		}
	}
	
	//������Ʈ ������ �ִ� Ÿ�ϵ� ����
	POINTFLOAT size = _tileImage->GetFrameSize();
	for (int i = 0; i < TILENUMY; i++)
	{
		for (int j = 0; j < TILENUMX; j++)
		{
			if (_tiles[i][j].haveObject == true)
			{
				_tileImage->SetSize(size);
				_tileImage->FrameRender(_tiles[i][j].rc.left, _tiles[i][j].rc.top,
					_tiles[i][j].objFrameX, _tiles[i][j].objFrameY, Pivot::LEFT_TOP);
			}
		}
	}
	
	//�������̽� ���� ��ư�� ����
	IMAGEMANAGER->FindImage("maptool_interface")->Render(_rcInterface.left, _rcInterface.top, Pivot::LEFT_TOP);

	if (_ctrlSelect == CTRL::SAVE)		IMAGEMANAGER->FindImage("maptool_save_click")->Render(_rcSave.left, _rcSave.top, Pivot::LEFT_TOP);
	else								IMAGEMANAGER->FindImage("maptool_save")->Render(_rcSave.left, _rcSave.top, Pivot::LEFT_TOP);
	if (_ctrlSelect == CTRL::LOAD)		IMAGEMANAGER->FindImage("maptool_load_click")->Render(_rcLoad.left, _rcLoad.top, Pivot::LEFT_TOP);
	else								IMAGEMANAGER->FindImage("maptool_load")->Render(_rcLoad.left, _rcLoad.top, Pivot::LEFT_TOP);
	if (_tileType == TILETYPE::OBJECT)  IMAGEMANAGER->FindImage("maptool_object_click")->Render(_rcObject.left, _rcObject.top, Pivot::LEFT_TOP);
	else								IMAGEMANAGER->FindImage("maptool_object")->Render(_rcObject.left, _rcObject.top, Pivot::LEFT_TOP);
	if (_tileType == TILETYPE::ERASER)  IMAGEMANAGER->FindImage("maptool_eraser_click")->Render(_rcEraser.left, _rcEraser.top, Pivot::LEFT_TOP);
	else								IMAGEMANAGER->FindImage("maptool_eraser")->Render(_rcEraser.left, _rcEraser.top, Pivot::LEFT_TOP);

	//Ÿ���� �巡�� �Ҷ� �巡���� ���� ��Ʈ�� �����ֱ�
	if (_isTileDrag)
	{
		RECT rc = { _tiles[_tileDragStart.y][_tileDragStart.x].rc.left, _tiles[_tileDragStart.y][_tileDragStart.x].rc.top,
			_tiles[_tileDragEnd.y][_tileDragEnd.x].rc.right, _tiles[_tileDragEnd.y][_tileDragEnd.x].rc.bottom };
		D2DRENDERER->DrawRectangle(rc, D2DRenderer::DefaultBrush::Red);
	}

	//����Ÿ�� ����
	_tileImage->SetSize({ _tileImage->GetSize().x * 3.0f, _tileImage->GetSize().y * 3.0f });
	_tileImage->Render(_sampleTile[0][0].rc.left, _sampleTile[0][0].rc.top, Pivot::LEFT_TOP);

	if (KEYMANAGER->IsToggleKey(VK_F2))
	{
		for (int i = 0; i < SAMPLETILEY; i++)
		{
			for (int j = 0; j < SAMPLETILEX; j++)
			{
				D2DRENDERER->DrawRectangle(_sampleTile[i][j].rc, D2DRenderer::DefaultBrush::Gray);
			}
		}
	}

	//����Ÿ�� �巡���Ҷ� ��Ʈ�� ���� �����ֱ�
	RECT rc = { _sampleTile[_sampleDragStart.y][_sampleDragStart.x].rc.left, _sampleTile[_sampleDragStart.y][_sampleDragStart.x].rc.top,
	_sampleTile[_sampleDragEnd.y][_sampleDragEnd.x].rc.right, _sampleTile[_sampleDragEnd.y][_sampleDragEnd.x].rc.bottom };
	D2DRENDERER->DrawRectangle(rc, D2DRenderer::DefaultBrush::Red);

	//���콺 �ε��� ����
	sprintf(str, "idX : %d, idY : %d", _mouseIdx.x, _mouseIdx.y);
	if ((_mouseIdx.x < 0 || _mouseIdx.x > 27) && (_mouseIdx.y < 0 || _mouseIdx.y > 35)) sprintf(str, "idX : %s, idY : %s", "out", "out");
	else if (_mouseIdx.x < 0 || _mouseIdx.x > 27) sprintf(str, "idX : %s, idY : %d", "out", _mouseIdx.y);
	else if (_mouseIdx.y < 0 || _mouseIdx.y > 35) sprintf(str, "idX : %d, idY : %s", _mouseIdx.x, "out");
	wstring wstr;
	for (int i = 0; str[i] != NULL; i++)
	{
		wstr.push_back(str[i]);
	}
	D2DRENDERER->RenderText(0, WINSIZEY - 30, wstr, 20, D2DRenderer::DefaultBrush::Red);

	
}

//���� �ʱ�ȭ
void MapTool::maptoolSetup()
{
	// ���� ��ư�� ��ġ �ʱ�ȭ
	_rcInterface = Figure::RectMake(350, 200, 490, 180);
	_rcSave = Figure::RectMake(_rcInterface.left + 20, _rcInterface.top + 200, 100, 30);
	_rcLoad = Figure::RectMake(_rcInterface.left + 20 + 110, _rcInterface.top + 200, 100, 30);
	_rcObject = Figure::RectMake(_rcInterface.left + 40 + 220, _rcInterface.top + 200, 100, 30);
	_rcEraser = Figure::RectMake(_rcInterface.left + 40 + 330, _rcInterface.top + 200, 100, 30);

	for (int i = 0; i < SAMPLETILEY; i++)
	{
		for (int j = 0; j < SAMPLETILEX; j++)
		{
			_sampleTile[i][j].frameX = j;
			_sampleTile[i][j].frameY = i;
			_sampleTile[i][j].rc = Figure::RectMake(_rcInterface.left + 45 + j * SAMPLETILESIZE,_rcInterface.top + 20 + i * SAMPLETILESIZE, SAMPLETILESIZE, SAMPLETILESIZE);
		}
	}
	for (int i = 0; i < TILENUMY; i++)
	{
		for (int j = 0; j < TILENUMX; j++)
		{
			_tiles[i][j].objFrameX = 0;
			_tiles[i][j].objFrameY = 0;
			_tiles[i][j].rc = Figure::RectMake(j * TILESIZE, i * TILESIZE + 120, TILESIZE, TILESIZE);
			_tiles[i][j].haveObject = false;
		}
	}
}

//��ư Ŭ��(���̺�, �ε�, ������Ʈ ��ġ, ������Ʈ �����)
void MapTool::clickButton()
{
	if (PtInRect(&_rcSave, _ptMouse))
	{
		_ctrlSelect = CTRL::SAVE;
		this->save();
	}
	else if (PtInRect(&_rcLoad, _ptMouse))
	{
		_ctrlSelect = CTRL::LOAD;
		this->load();
	}
	else if (PtInRect(&_rcObject, _ptMouse))
	{
		_tileType = TILETYPE::OBJECT;
	}
	else if (PtInRect(&_rcEraser, _ptMouse))
	{
		_tileType = TILETYPE::ERASER;
	}
}

//Ÿ�� ��ġ(�巡�� ����)
void MapTool::setMap()
{
	if (PtInRect(&_tiles[_mouseIdx.y][_mouseIdx.x].rc, _ptMouse))
	{
		//����Ÿ���� 1���� �������� ���
		if (_sampleDragNumX == 0 && _sampleDragNumY == 0)
		{
			if (KEYMANAGER->IsStayKeyDown(VK_SHIFT))
			{
				_tileDragEnd.x = _mouseIdx.x;
				_tileDragEnd.y = _mouseIdx.y;
				_isTileDrag = true;
			}
			else
			{
				if (_tileType == TILETYPE::OBJECT)
				{
					_tiles[_mouseIdx.y][_mouseIdx.x].objFrameX = _sampleDragStart.x;
					_tiles[_mouseIdx.y][_mouseIdx.x].objFrameY = _sampleDragStart.y;
					_tiles[_mouseIdx.y][_mouseIdx.x].haveObject = true;
				}
				else if (_tileType == TILETYPE::ERASER)
				{
					_tiles[_mouseIdx.y][_mouseIdx.x].haveObject = false;
				}

			}
		}
		//����Ÿ�� �巡���ؼ� ������ �������� ���
		else
		{
			for (int k = 0; k < _sampleDragNumY + 1; k++)
			{
				for (int g = 0; g < _sampleDragNumX + 1; g++)
				{
					if (_tileType == TILETYPE::OBJECT)
					{
						_tiles[_mouseIdx.y + k][_mouseIdx.x + g].objFrameX = g + _sampleDragStart.x;
						_tiles[_mouseIdx.y + k][_mouseIdx.x + g].objFrameY = k + _sampleDragStart.y;
						_tiles[_mouseIdx.y + k][_mouseIdx.x + g].haveObject = true;
					}
					else
					{
						_tiles[_mouseIdx.y + k][_mouseIdx.x + g].haveObject = false;
					}
				}
			}
		}
	}
}

//Ÿ�� �巡�� �Լ�
void MapTool::tileDrag()
{
	if (PtInRect(&_tiles[_mouseIdx.y][_mouseIdx.x].rc, _ptMouse))
	{
		_tileDragStart.x = _mouseIdx.x;
		_tileDragStart.y = _mouseIdx.y;
	}
	_isTileDrag = false;
}

//�� ������ ���̺�
void MapTool::save()
{
	HANDLE file;
	DWORD write;
	file = CreateFile(L"./tile.map", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(file, _tiles, sizeof(tagTile) * TILENUMX * TILENUMY, &write, NULL);
	CloseHandle(file);
}

//�� ������ �ε�
void MapTool::load()
{
	HANDLE file;
	DWORD read;
	file = CreateFile(L"./tile.map", GENERIC_READ, 0, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, _tiles, sizeof(tagTile) * TILENUMX * TILENUMY, &read, NULL);
	CloseHandle(file);
	
}

