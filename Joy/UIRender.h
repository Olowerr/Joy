#pragma once
#include<vector>

#include "Sprite.h" 
#include "Text.h"

class UIRenderer
{
public:
	UIRenderer();
	void Shutdown();

	void Clear();
	void Add(Sprite* sprite);
	void Add(Text* text);
	void Draw();
	void DisableAll();
	void EnableAll();

	static const UINT NumVertices = 4;
	
private:
	std::vector<Sprite*> sprites;
	std::vector<Text*> texts;

	DirectX::SpriteBatch spriteBatch;
	const wchar_t fontPath[41] = L"../Resources/TextStuff/myfile.spritefont";

	ID3D11Buffer* quadBuffer;
	ID3D11Buffer* projection;

	//ID3D11ShaderResourceView* uiImgSRV;
	ID3D11InputLayout* UI_IL;
	ID3D11VertexShader* UI_VS;
	ID3D11RasterizerState* UI_RS;
	ID3D11PixelShader* UI_PS;
};