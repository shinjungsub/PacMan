#include "stdafx.h"
#include "ObjectManager.h"

#include "GameObject.h"

#include <algorithm>
#include <unordered_map> 

/*********************************************************************
## ObjectManager ##
미리 카테고리 별로 벡터 넣어논다.
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
모든 오브젝트들 메모리 해제 및 컨테이너도 비워준다.
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
모든 오브젝트 할당 해제, 및 벡터 clear(이때 맵은 clear해주면 안된다)
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
@@ ObjectType::Enum type : 오브젝트 타입
@@ GameObject* pObject : 오브젝트
**********************************************************************/
void ObjectManager::AddObject(const ObjectType::Enum type, GameObject* pObject)
{
	this->_objectContainer[type].push_back(pObject);
}
/*********************************************************************
## FindObject ##
@@ ObjectType::Enum type : 타입
@@ string name : 찾을 오브젝트 이름
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
@@ ObjectType::Enum type : 찾을 오브젝트 타입
@@ string name : 이름
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
@@ ObjectType::Enum type : 오브젝트 타입
**********************************************************************/
vector<class GameObject*> ObjectManager::GetObjectList(const ObjectType::Enum type)
{
	return _objectContainer[type];
}
/*********************************************************************
## GetObjectListPoiter ##
해당 타입의 오브젝트를 담은 벡터 포인터를 가져온다. 가져올 때는 const vector<class GameObject*>* 로 가져오는데
포인터로 가져오는 이유는 밸류반환을 통해 벡터의 모든 원소들을 복사해서 가져오지 않기 위해서다. 매 프레임마다
GetObjectList를 통해 벡터를 반환하게 되면 값복사가 일어나고 오브젝트의 수가 많아질 수록 느려진다.

두번째로 const를 붙여서 const vector<GameObject*>*로 가져오는 이유는 외부에서 포인터로 접근해서 함부로
벡터의 원소를 삭제하거나 추가하는 등의 안정성을 해칠 우려를 없애기 위해서다.

@@ ObjectType::Enum type : 오브젝트 타입
**********************************************************************/
const vector<class GameObject*>* ObjectManager::GetObjectListPointer(const ObjectType::Enum type)
{
	return &_objectContainer[type];
}

