#pragma once
#include "UIElement.h"

class Sprite : UIElement
{
public:
	Sprite(const std::string& imagePath, FLOAT xPos, FLOAT yPos, FLOAT xScale, FLOAT yScale);
	Sprite(const std::string& imagePath);
	virtual void Shutdown() override;
	~Sprite();

	virtual void SetPosition(FLOAT X, FLOAT Y) override;
	void SetScale(FLOAT x, FLOAT y);

	bool Hovered();
	bool Clicked();

	void Draw();

private:

	const bool immutable;

	DirectX::XMINT2 initialSize;
	DirectX::XMFLOAT2 size;

	ID3D11ShaderResourceView* imageSRV;
	ID3D11Buffer* transformBuffer;

	bool Load(const std::string& imagePath);
};