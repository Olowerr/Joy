#pragma once
#include <fstream>
#include <iostream>
#include "Backend.h"
#include "Object.h"
#include "CameraBase.h"

struct InstancedObjects
{
	ID3D11Buffer* vertexBuffer;
	ID3D11ShaderResourceView* transformSRV;
	ID3D11ShaderResourceView* mtl;
	UINT instanceCount;
	UINT indexCount;

	void Shutdown()
	{
		transformSRV->Release();
		// Don't release vertexBuffer, meshStorage owns it
		// Don't release mtl, meshStorage owns it
	}

};

class ObjectRender
{
public:
	ObjectRender();
	void Shutdown();
	void Clear();

	void SetActiveCamera(Camera* camera);

	void CreateSamplerState(); // << temporary
	void AddObject(Object* obj);
	void DrawAll();

	void CreateCharacterDecal(Character* character);
	void UpdateCharacterDecal(Character* character);
	ID3D11Buffer* const* getDecalBuffer();

	// Add Instanced Objects
	bool GiveInstancedObjects(Object* objArr, UINT amount);

	ID3D11InputLayout* GetObjectInputLayout();
	ID3D11VertexShader* GetObjectVS();

private:

	std::vector<Object*> objects;

	ID3D11InputLayout* inpLayout;
	ID3D11RenderTargetView* const* bbRTV;

	// Decal buffer with character position, updates constantly
	ID3D11Buffer* charPosBuff;

	// Instanced
	ID3D11VertexShader* objInstanceVS;
	std::vector<InstancedObjects> instances;

	// Normal
	ID3D11VertexShader* objVS;
	ID3D11PixelShader* objPS;

	// Sampler
	ID3D11SamplerState* sampler;  // << temporary
	
	
	bool LoadShaders();
	bool CreateInputLayout(const std::string& shaderData);

	
};