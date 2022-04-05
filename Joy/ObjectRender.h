#pragma once
#include <fstream>
#include <iostream>
#include "Backend.h"
#include "Object.h"

class ObjectRender
{
public:

	ObjectRender();
	void Shutdown();

	void Add(Object* obs);
	void DrawAll();

private:

	std::vector<Object*> obs;

	ID3D11InputLayout* inpLayout;

	ID3D11VertexShader* objVS;
	ID3D11PixelShader* objPS;
	ID3D11RenderTargetView* const* bbRTV;

	std::string vShaderByteCode;

	bool LoadShaders();
	bool CreateInputLayout(const std::string& shaderData);

	// temp
	ID3D11Buffer* cam;
};