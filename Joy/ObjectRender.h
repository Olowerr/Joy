#pragma once
#include <fstream>
#include <iostream>
#include "Backend.h"
#include "Object.h"

struct InstanceResource
{
	ID3D11Buffer* vertexBuffer;
	//ID3D11Buffer* indexBuffer; 
	ID3D11ShaderResourceView* transformSRV;
	//ID3D11ShaderResourceView* lightMapsSRV;
	UINT instanceCount;
	UINT indexCount;

	void Shutdown()
	{
		transformSRV->Release();
		//Don't release vertexBuffer, meshStorage owns it
	}

};

class ObjectRender
{
public:
	ObjectRender();
	void Shutdown();
	void Clear();

	void AddObject(Object* obj);
	void DrawAll();

	// Add Instanced Objects
	bool AddInstancedObjects(Object* objArr, UINT amount);


private:

	std::vector<Object*> objects;

	ID3D11InputLayout* inpLayout;
	ID3D11RenderTargetView* const* bbRTV;

	// Instanced
	ID3D11VertexShader* objInstanceVS;
	ID3D11PixelShader* objInstancePS;
	std::vector<InstanceResource> instances;

	// Normal
	ID3D11VertexShader* objVS;
	ID3D11PixelShader* objPS;

	bool LoadShaders();
	bool CreateInputLayout(const std::string& shaderData);


	//temp
	ID3D11Buffer* cam; 
};