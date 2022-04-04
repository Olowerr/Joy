#pragma once
#include "UIElement.h" // sprite

#include<vector>

class UIRender
{
public:
	UIRender() = default;
	void Initiate();
	void Shutdown();

	void Add(UIElement* element);
	void Draw();

private:
	std::vector<UIElement*> elements;

	ID3D11Buffer* quadBuffer;

	ID3D11InputLayout* UI_IL;
	ID3D11VertexShader* UI_VS;
	ID3D11RasterizerState* UI_RS;
	ID3D11PixelShader* UI_PS;



};