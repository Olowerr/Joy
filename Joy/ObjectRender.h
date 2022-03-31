#pragma once
#include <d3d11.h>
#include <fstream>
#include <iostream>

class ObjectRender
{
public:

	ObjectRender();

	~ObjectRender();

	bool LoadShaders(ID3D11Device* device);
	bool CreateInputLayout(ID3D11Device* device);

private:

	D3D11_VIEWPORT viewPort;

	ID3D11InputLayout* inpLayout;

	ID3D11VertexShader* objVS;
	ID3D11PixelShader* objPS;

	std::string vShaderByteCode;
};