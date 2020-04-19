#include "stdafx.h"
#include "ObjectManager.h"

#include "GameObject.h"
#include <algorithm>

void ObjectManager::ZOrder()
{
	//오름차순 기준으로 zorder
	sort(_objectContainer[ObjectType::Object].begin(), _objectContainer[ObjectType::Object].end(),
		[this](GameObject* object1, GameObject* object2)
	{
		if (object1->GetCollisionRect().bottom < object2->GetCollisionRect().bottom)
			return true;
		return false;
	});
}

void ObjectManager::ObjectRender()
{
	if (_isZorder)
		this->ZOrder();

	ObjectContainerIter iter = _objectContainer.begin();
	for (; iter != _objectContainer.end(); ++iter)
	{
		if (iter->first == ObjectType::UI)
			continue;
		for (UINT i = 0; i < iter->second.size(); ++i)
		{
			if (iter->second[i]->GetActive())
			{
				iter->second[i]->Render();
			}
		}
	}
}

void ObjectManager::UIRender()
{
	for (UINT i = 0; i < _objectContainer[ObjectType::UI].size(); ++i)
	{
		GameObject* object = _objectContainer[ObjectType::UI][i];
		if (object->GetActive())
			object->Render();
	}
}

