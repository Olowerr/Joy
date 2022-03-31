#pragma once
#include <fstream>
#include <iostream>
#include "Backend.h"

class ObjectRender
{
public:

	ObjectRender();

	~ObjectRender();

	bool LoadShaders();
	bool CreateInputLayout();
	void SetViewPort();

private:

	D3D11_VIEWPORT viewPort;

	ID3D11InputLayout* inpLayout;

	ID3D11VertexShader* objVS;
	ID3D11PixelShader* objPS;

	std::string vShaderByteCode;

	Backend& backend;
};