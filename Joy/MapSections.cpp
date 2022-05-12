#include "MapSections.h"

MapDivider::MapDivider(Character& joy)
	:numSections(0), joy(joy), activeSection(&nullSection)
{

}

MapDivider::~MapDivider()
{

}

void MapDivider::Shutdown()
{
	delete[] sections;
}

void MapDivider::CreateSections(UINT numSections, float mapLength, float mapWidth, float mapHeight)
{
	float sectionZSize = (mapLength / (float)(numSections));
	const std::vector<Object*>& levelObjects = Object::GetLevelObjects();
	const std::vector<Object*>& enviormentObjects = Object::GetEnviormentObjects();


	sections = new Section[numSections];
	this->numSections = numSections;

	for (UINT i = 0; i < numSections; i++)
	{
		DirectX::BoundingBox& currentBB = sections[i].sectionBB;

		currentBB.Center.z = (sectionZSize - sectionZSize * 0.5f) + sectionZSize * i;
		currentBB.Center.y = mapHeight * 0.5f;
		currentBB.Extents.x = mapWidth;		// Can be large just incase ;))))))
		currentBB.Extents.y = mapHeight;	// Can be large just incase ;))))))
		currentBB.Extents.z = sectionZSize * 0.5f;

		for (Object* object : levelObjects)
		{
			if (object->IsLevelObject)
			{
				if (currentBB.Intersects(object->GetBoundingBox(0)))
					sections[i].levelObjects.emplace_back(object);
			}
		}

		for (Object* object : enviormentObjects)
		{
			if (!object->IsLevelObject)
			{
				if (currentBB.Intersects(object->GetBoundingBox(0)))
					sections[i].enivormentObjects.emplace_back(object);
			}
		}
	}

	activeSection = &sections[0];
}

void MapDivider::Update()
{
	activeSection = &nullSection;
	return;

	for (UINT i = 0; i < numSections; i++)
	{
		if (sections[i].sectionBB.Intersects(joy.GetBoundingBox(0)))
		{
			activeSection = &sections[i];
			return;
		}
	}
	activeSection = &nullSection;
}

UINT MapDivider::GetNumSections() const
{
	return numSections;
}

Section* const MapDivider::GetSections() const
{
	return sections;
}

Section* const* MapDivider::GetActiveSection()
{
	return &activeSection;
}
