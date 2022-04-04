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

	void initiate();

	void Add(Object* obs);
	void DrawAll();

private:

	std::vector<Object*> obs;

	D3D11_VIEWPORT viewPort;

	ID3D11InputLayout* inpLayout;

	ID3D11VertexShader* objVS;
	ID3D11PixelShader* objPS;

	std::string vShaderByteCode;

	bool LoadShaders();
	bool CreateInputLayout(const std::string& shaderData);
	void SetViewPort();

	// temp
	ID3D11Buffer* cam;
	ID3D11RenderTargetView* bbRTV;
};