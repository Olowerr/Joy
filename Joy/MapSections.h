#pragma once
#include "Object.h"

struct Section
{
	std::vector<Object*> levelObjects;
	std::vector<Object*> enivormentObjects;

	DirectX::BoundingBox sectionBB;
};

class MapDivider
{
public:
	MapDivider(UINT numSections, float mapLength, float mapWidth, float mapHeight);
	~MapDivider();
	void Shutdown();

	UINT GetNumSections() const;
	Section* const GetSections() const;

private:
	Section* sections;
	const UINT numSections;
};