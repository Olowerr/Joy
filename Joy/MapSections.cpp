#include "MapSections.h"

MapDivider::MapDivider(UINT numSections, float mapLength, float mapWidth, float mapHeight)
	:numSections(numSections)
{
	float sectionZSize = (mapLength / (float)(numSections + 1));
	const std::vector<Object*>& levelObjects = Object::GetLevelObjects();
	const std::vector<Object*>& enviormentObjects = Object::GetEnviormentObjects();


	sections = new Section[numSections];
	for (UINT i = 0; i < numSections; i++)
	{
		DirectX::BoundingBox& currentBB = sections[i].sectionBB;

		currentBB.Center.z = sectionZSize * (i + 1);
		currentBB.Center.y = mapHeight * 0.5f;
		currentBB.Extents.x = mapWidth * 0.5f;
		currentBB.Extents.y = mapHeight * 0.5f;
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
}

MapDivider::~MapDivider()
{
}

void MapDivider::Shutdown()
{
	delete[] sections;
}

UINT MapDivider::GetNumSections() const
{
	return numSections;
}

Section* const MapDivider::GetSections() const
{
	return sections;
}