#pragma once
#include <fstream>
#include <iostream>
#include "Backend.h"
#include "Object.h"
#include "CharacterCamera.h"

struct InstancedObjects
{
	ID3D11Buffer* vertexBuffer;
	//ID3D11Buffer* indexBuffer; 
	ID3D11ShaderResourceView* transformSRV;
	ID3D11ShaderResourceView* mtl;
	//ID3D11ShaderResourceView* lightMapsSRV;
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

	void AddObject(Object* obj);
	void DrawAll();

	void CreateCharacterDecal(Character* character);
	void UpdateCharacterDecal(Character* character);
	ID3D11Buffer* const* getDecalBuffer();

	// Add Instanced Objects
	bool GiveInstancedObjects(Object* objArr, UINT amount);


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

	bool LoadShaders();
	bool CreateInputLayout(const std::string& shaderData);


	//temp
//	CharacterCamera* camera;
//	ID3D11Buffer* camCb;
//	DirectX::XMFLOAT4X4 viewAndProj;
};