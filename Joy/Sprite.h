#pragma once
#include "Backend.h"

#include<DirectXMath.h>

class Sprite
{
public:
	Sprite(const std::string& imagePath, FLOAT xPos, FLOAT yPos, FLOAT xScale, FLOAT yScale);
	Sprite(const std::string& imagePath);
	void Shutdown();
	~Sprite();


	void SetPosition(FLOAT x, FLOAT y);
	void SetScale(FLOAT x, FLOAT y);

	bool Hovered();
	bool Clicked();

	void Draw();

private:

	const bool immutable;

	DirectX::XMINT2 initialSize;
	DirectX::XMFLOAT2 pos;
	DirectX::XMFLOAT2 size;

	ID3D11ShaderResourceView* imageSRV;
	ID3D11Buffer* transformBuffer;

	bool Load(const std::string& imagePath);
};