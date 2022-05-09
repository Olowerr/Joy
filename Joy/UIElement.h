#pragma once
#include "Backend.h"
#include <DirectXMath.h>

class UIElement abstract
{
public:
	UIElement()
		:position()
	{
	}
	virtual void Shutdown() { };

	virtual void SetPosition(FLOAT X, FLOAT Y)
	{
		position.x = X;
		position.y = Y;
	}
	
	virtual void Draw() = 0;

protected:
	DirectX::XMFLOAT2 position;
};