#pragma once
#include <fstream>
#include <iostream>
#include "Backend.h"
#include "Object.h"
#include "CharacterCamera.h"
#include "DecalShadow.h"

class ObjectRender
{
public:
	ObjectRender();
	void Shutdown();
	void Clear();

	void AddObject(Object* obj);
	void DrawAll();

	// Add Instanced Objects
	bool GiveInstancedObjects(Object* objArr, UINT amount);


private:

	DecalShadow decalShadow;
	std::vector<Object*> objects;

	ID3D11InputLayout* inpLayout;
	ID3D11RenderTargetView* const* bbRTV;

	// Instanced
	ID3D11VertexShader* objInstanceVS;
	std::vector<InstancedObject> instances;

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