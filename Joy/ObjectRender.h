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
		//Don't release vertexBuffer, MeshStorage does that
	}

};

class ObjectRender
{
public:
public:
	ObjectRender();
	void Shutdown();

	void AddDynamic(Object* obj);
	void DrawAll();

	// Add Static Objects
	void AddStatic(Object* obj);

	// Finalize the current Static Objects
	bool FinalizeCurrentStatic();


private:

	std::vector<Object*> objects;
	std::vector<Object*> staticObjects;

	ID3D11InputLayout* inpLayout;
	ID3D11RenderTargetView* const* bbRTV;

	// Instanced
	ID3D11VertexShader* objInstanceVS;
	ID3D11PixelShader* objInstancePS;
	std::vector<InstanceResource> instances;

	// Normal
	ID3D11VertexShader* objVS;
	ID3D11PixelShader* objPS;

	ID3D11Buffer* cam;

	bool LoadShaders();
	bool CreateInputLayout(const std::string& shaderData);
};