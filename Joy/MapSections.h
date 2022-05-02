#pragma once
#include "Character.h"

struct Section
{
	std::vector<Object*> levelObjects;
	std::vector<Object*> enivormentObjects;

	DirectX::BoundingBox sectionBB;
};

class MapDivider
{
public:
	MapDivider(Character& joy, UINT numSections, float mapLength, float mapWidth, float mapHeight);
	~MapDivider();
	void Shutdown();

	UINT GetNumSections() const;
	Section* const GetSections() const;
	const Section* GetActiveSection() const;

private:
	Character& joy;
	Section* sections;
	const UINT numSections;
};