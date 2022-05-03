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
	MapDivider(Character& joy);
	~MapDivider();
	void Shutdown();

	void CreateSections(UINT numSections, float mapLength, float mapWidth, float mapHeight);
	void Update();

	UINT GetNumSections() const;
	Section* const GetSections() const;
	Section* const * GetActiveSection();

private:
	Character& joy;
	Section* sections;
	UINT numSections;

	Section* activeSection;
	Section nullSection;
};