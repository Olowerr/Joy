#pragma once
#include "Character.h"
#include "imguiSetup.h"

class HelpComputer : public Object
{
public:
	HelpComputer(Mesh* mesh, const Character& joy);

	bool Check();

	void SetTrigger(DirectX::XMFLOAT3 center, DirectX::XMFLOAT3 extents);

private:
	const Character& joy;
	DirectX::BoundingBox trigger;

	float timePassed;
};