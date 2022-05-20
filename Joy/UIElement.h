#pragma once
#include "Backend.h"
#include <DirectXMath.h>

class UIElement abstract
{
public:
	UIElement()
		:position(), active(true)
	{
	}
	virtual void Shutdown() { };

	virtual void SetPosition(FLOAT X, FLOAT Y)
	{
		position.x = X;
		position.y = Y;
	}
	
	virtual void Draw() = 0;
	
	void SetActive(bool active)
	{
		this->active = active;
	}

protected:
	DirectX::XMFLOAT2 position;
	bool active;
};