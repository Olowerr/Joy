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
	void CreateSamplerState(); // << temporary
	void SetActiveCamera(Camera* camera);
	void SetMapDivier(MapDivider* sections);
	void DrawAll();

	// Add Instanced Objects
	bool GiveInstancedObjects(Object* objArr, UINT amount);

private:

	MapDivider* sections;
	const Section* const* activeSection;

	GraphicsStorage& storage;

	ID3D11RenderTargetView* const* bbRTV;

	// Instanced
	std::vector<InstancedObject> instances;

	// Sampler
	ID3D11SamplerState* sampler;  // << temporary

	Camera* activeCamera;

	//temp
//	CharacterCamera* camera;
//	ID3D11Buffer* camCb;
//	DirectX::XMFLOAT4X4 viewAndProj;
};