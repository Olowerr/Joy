#pragma once
#include "Backend.h"

#include<DirectXMath.h>

class UIElement abstract
{
public:
	UIElement(DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 size);

	bool Hovered();
	bool Clicked();

	void Draw();


private:
	const DirectX::XMFLOAT2 pos;
	const DirectX::XMFLOAT2 size;

	ID3D11Buffer* transformBuffer;


	virtual void BindResource() = 0;
};