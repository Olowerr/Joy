#include "Text.h"

Text::Text()
	:UIElement(), scale(1.f, 1.f), colour(DirectX::Colors::White), text(), spriteBatch(nullptr), sprite(nullptr)
{
}

Text::Text(const std::string& text)
	:UIElement(), scale(1.f, 1.f), colour(DirectX::Colors::White), text(text), spriteBatch(nullptr), sprite(nullptr)
{
}

void Text::Shutdown()
{
	if (sprite)
	{
		delete sprite;
		sprite = nullptr;
	}
}

void Text::Initiate(DirectX::SpriteBatch* spriteBatch, const wchar_t* spriteFont)
{
	this->spriteBatch = spriteBatch;

	sprite = new DirectX::SpriteFont(Backend::GetDevice(), spriteFont);
}

void Text::SetText(const std::string& text)
{
	this->text = text;
}

void Text::SetColour(DirectX::XMVECTOR colour)
{
	this->colour = colour;
}

void Text::SetScale(FLOAT X, FLOAT Y)
{
	scale.x = X;
	scale.y = Y;
}

void Text::Draw()
{
	if (active)
		sprite->DrawString(spriteBatch, text.c_str(), position, colour, 0.f, DirectX::XMFLOAT2(), scale);
}
