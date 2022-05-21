#pragma once
#include "Character.h"
#include "imguiSetup.h"
#include "UIRender.h"

class HelpComputer : public Object
{
public:
	HelpComputer(Mesh* mesh, const Character& joy, UIRenderer& uiRender);
	void Shutdown() override;

	bool Check();

	void SetTrigger(DirectX::XMFLOAT3 center, DirectX::XMFLOAT3 extents);
	void SetActive(bool active);

private:
	const Character& joy;
	UIRenderer& uiRender;

	Sprite sprite;
	bool showing;

	DirectX::BoundingBox trigger;

	DirectX::XMFLOAT3 startPos;

	float timePassed;
};