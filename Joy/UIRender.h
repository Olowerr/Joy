#pragma once
#include<vector>

#include "Sprite.h" 

class UIRenderer
{
public:
	UIRenderer();
	void Shutdown();

	void Clear();
	void Add(Sprite* element);
	void Finalize();
	void Draw();

	static const UINT NumVertices = 4;
	
private:
	std::vector<Sprite*> elements;

	ID3D11Buffer* quadBuffer;
	ID3D11Buffer* projection;

	ID3D11ShaderResourceView* uiImgSRV;
	ID3D11InputLayout* UI_IL;
	ID3D11VertexShader* UI_VS;
	ID3D11RasterizerState* UI_RS;
	ID3D11PixelShader* UI_PS;

	ID3D11RenderTargetView* const* bbRTV;


};