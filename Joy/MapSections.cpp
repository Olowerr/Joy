#include "MapSections.h"

MapDivider::MapDivider(Character& joy, UINT numSections, float mapLength, float mapWidth, float mapHeight)
	:numSections(numSections), joy(joy), activeSection(&nullSection)
{
	float sectionZSize = (mapLength / (float)(numSections));
	const std::vector<Object*>& levelObjects = Object::GetLevelObjects();
	const std::vector<Object*>& enviormentObjects = Object::GetEnviormentObjects();


	sections = new Section[numSections];
	for (UINT i = 0; i < numSections; i++)
	{
		DirectX::BoundingBox& currentBB = sections[i].sectionBB;

		currentBB.Center.z = (sectionZSize - sectionZSize * 0.5f) * (i + 1);
		currentBB.Center.y = mapHeight * 0.5f;
		currentBB.Extents.x = mapWidth;		// Can be large just incase ;))))))
		currentBB.Extents.y = mapHeight;	// Can be large just incase ;))))))
		currentBB.Extents.z = sectionZSize * 0.5f;
		
		for (Object* object : levelObjects)
		{
			if (object->IsLevelObject)
			{
				if (currentBB.Intersects(object->GetBoundingBox()))
					sections[i].levelObjects.emplace_back(object);
			}
		}
		
		for (Object* object : enviormentObjects)
		{
			if (!object->IsLevelObject)
			{
				if (currentBB.Intersects(object->GetBoundingBox()))
					sections[i].enivormentObjects.emplace_back(object);
			}
		}
	}

	activeSection = &sections[0];
}

MapDivider::~MapDivider()
{

}

void MapDivider::Shutdown()
{
	delete[] sections;
}

void MapDivider::Update()
{
	activeSection = &nullSection;
	return;

	for (UINT i = 0; i < numSections; i++)
	{
		if (sections[i].sectionBB.Intersects(joy.GetBoundingBox()))
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
