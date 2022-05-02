#pragma once
#include "Character.h"

struct Section
{
	Section() = default;

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

	void Update();

	UINT GetNumSections() const;
	Section* const GetSections() const;
	Section* const * GetActiveSection();

private:
	Character& joy;
	Section* sections;
	const UINT numSections;

	Section* activeSection;
	Section nullSection;
};