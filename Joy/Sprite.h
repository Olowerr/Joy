#pragma once
#include "Backend.h"
#include "stb_image.h"

#include<DirectXMath.h>

typedef class Sprite
{
public:
	static Sprite* Create(const std::string& imagePath, FLOAT xPos, FLOAT yPos, FLOAT xScale, FLOAT yScale);
	static Sprite* Create(const std::string& imagePath);

	Sprite(const Sprite&) = delete;
	Sprite(Sprite&&) = delete;
	Sprite& operator= (const Sprite&) = delete;

	void Shutdown();

	void SetPosition(FLOAT x, FLOAT y);
	void SetScale(FLOAT x, FLOAT y);

	bool Hovered();
	bool Clicked();

	void Draw();

private:
	Sprite(const std::string& imagePath, FLOAT xPos, FLOAT yPos, FLOAT xScale, FLOAT yScale);
	Sprite(const std::string& imagePath);

	const bool immutable;

	DirectX::XMINT2 initialSize;
	DirectX::XMFLOAT2 pos;
	DirectX::XMFLOAT2 size;

	ID3D11ShaderResourceView* imageSRV;
	ID3D11Buffer* transformBuffer;

	bool Load(const std::string& imagePath);
}*PtrSprite;