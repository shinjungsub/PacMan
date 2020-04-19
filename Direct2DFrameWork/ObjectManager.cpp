#include "stdafx.h"
#include "ObjectManager.h"

#include "GameObject.h"

#include <algorithm>
#include <unordered_map> 

/*********************************************************************
## ObjectManager ##
�̸� ī�װ� ���� ���� �־���.
**********************************************************************/
ObjectManager::ObjectManager()
	:_isZorder(true)
{
	for (UINT i = 0; i < ObjectType::End; ++i)
	{
		_objectContainer.insert(make_pair((ObjectType::Enum)i, vector<GameObject*>()));
	}

}
/*********************************************************************
## ~ObjectManager ##
��� ������Ʈ�� �޸� ���� �� �����̳ʵ� ����ش�.
**********************************************************************/
ObjectManager::~ObjectManager()
{
	ObjectContainerIter iter = _objectContainer.begin();
	for (; iter != _objectContainer.end(); ++iter)
	{
		for (UINT i = 0; i < iter->second.size(); ++i)
		{
			iter->second[i]->Release();
			SAFE_DELETE(iter->second[i]);
		}
		iter->second.clear();
	}
	_objectContainer.clear();
}
/*********************************************************************
## Init ##
**********************************************************************/
void ObjectManager::Init()
{
	ObjectContainerIter iter = _objectContainer.begin();
	for (; iter != _objectContainer.end(); ++iter)
	{
		for (UINT i = 0; i < iter->second.size(); ++i)
		{
			iter->second[i]->Init();
		}
	}
}
/*********************************************************************
## Release ##
��� ������Ʈ �Ҵ� ����, �� ���� clear(�̶� ���� clear���ָ� �ȵȴ�)
**********************************************************************/
void ObjectManager::Release()
{
	ObjectContainerIter iter = _objectContainer.begin();
	for (; iter != _objectContainer.end(); ++iter)
	{
		for (UINT i = 0; i < iter->second.size(); ++i)
		{
			iter->second[i]->Release();
			SAFE_DELETE(iter->second[i]);
		}
		iter->second.clear();
	}
}
/*********************************************************************
## Update ##
**********************************************************************/
void ObjectManager::Update()
{
	ObjectContainerIter iter = _objectContainer.begin();
	for (; iter != _objectContainer.end(); ++iter)
	{
		for (UINT i = 0; i < iter->second.size(); ++i)
		{
			if (iter->second[i]->GetLive() == false)
			{
				iter->second[i]->Release();
				SAFE_DELETE(iter->second[i]);
				iter->second.erase(iter->second.begin() + i--);
				continue;
			}
			if (iter->second[i]->GetActive())
			{
				iter->second[i]->Update();
			}
		}
	}
}
/*********************************************************************
## Render ##
**********************************************************************/
void ObjectManager::Render()
{
	this->ObjectRender();
	this->UIRender();
}
/*********************************************************************
## AddObject ##
@@ ObjectType::Enum type : ������Ʈ Ÿ��
@@ GameObject* pObject : ������Ʈ
**********************************************************************/
void ObjectManager::AddObject(const ObjectType::Enum type, GameObject* pObject)
{
	this->_objectContainer[type].push_back(pObject);
}
/*********************************************************************
## FindObject ##
@@ ObjectType::Enum type : Ÿ��
@@ string name : ã�� ������Ʈ �̸�
**********************************************************************/
GameObject* ObjectManager::FindObject(const ObjectType::Enum type, const string name)
{
	ObjectContainerIter iter = _objectContainer.find(type);
	for (UINT i = 0; i < iter->second.size(); ++i)
	{
		if (iter->second[i]->GetName() == name)
		{
			return iter->second[i];
		}
	}
	return nullptr;
}
/*********************************************************************
## FindObjects ##
@@ ObjectType::Enum type : ã�� ������Ʈ Ÿ��
@@ string name : �̸�
**********************************************************************/
vector<class GameObject*> ObjectManager::FindObjects(const ObjectType::Enum type, const string name)
{
	vector<GameObject*> findList;
	ObjectContainerIter iter = _objectContainer.find(type);
	for (UINT i = 0; i < iter->second.size(); ++i)
	{
		if (iter->second[i]->GetName() == name)
		{
			findList.push_back(iter->second[i]);
		}
	}

	return findList;
}
/*********************************************************************
## GetObjectList ##
@@ ObjectType::Enum type : ������Ʈ Ÿ��
**********************************************************************/
vector<class GameObject*> ObjectManager::GetObjectList(const ObjectType::Enum type)
{
	return _objectContainer[type];
}
/*********************************************************************
## GetObjectListPoiter ##
�ش� Ÿ���� ������Ʈ�� ���� ���� �����͸� �����´�. ������ ���� const vector<class GameObject*>* �� �������µ�
�����ͷ� �������� ������ �����ȯ�� ���� ������ ��� ���ҵ��� �����ؼ� �������� �ʱ� ���ؼ���. �� �����Ӹ���
GetObjectList�� ���� ���͸� ��ȯ�ϰ� �Ǹ� �����簡 �Ͼ�� ������Ʈ�� ���� ������ ���� ��������.

�ι�°�� const�� �ٿ��� const vector<GameObject*>*�� �������� ������ �ܺο��� �����ͷ� �����ؼ� �Ժη�
������ ���Ҹ� �����ϰų� �߰��ϴ� ���� �������� ��ĥ ����� ���ֱ� ���ؼ���.

@@ ObjectType::Enum type : ������Ʈ Ÿ��
**********************************************************************/
const vector<class GameObject*>* ObjectManager::GetObjectListPointer(const ObjectType::Enum type)
{
	return &_objectContainer[type];
}

