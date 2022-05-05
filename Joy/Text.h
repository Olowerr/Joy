#pragma once
#include "UIElement.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"

class Text : public UIElement
{
public:
	Text();
	Text(const std::string& text);
	virtual void Shutdown() override;

	void Initiate(DirectX::SpriteBatch* spriteBatch, const wchar_t* spriteFont);
	void SetText(const std::string& text);
	void SetColour(DirectX::XMVECTOR colour); // Uses DirectX::Colors::
	void SetScale(FLOAT X, FLOAT Y);
	virtual void Draw() override;

private:
	DirectX::SpriteBatch* spriteBatch;
	DirectX::SpriteFont* sprite;
	std::string text;
	DirectX::XMVECTOR colour;
	DirectX::XMFLOAT2 scale;
};