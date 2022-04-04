#pragma once
#include "Backend.h"

#include<DirectXMath.h>

class UIElement abstract
{
public:
	UIElement(const std::string& imagePath, DirectX::XMFLOAT2 pos);

	bool Hovered();
	bool Clicked();

	void Draw();

private:
	const DirectX::XMFLOAT2 pos;
	const DirectX::XMFLOAT2 size;

	ID3D11Buffer* transformBuffer;


	virtual void BindResource() = 0;
};