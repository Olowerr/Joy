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
	void DrawCharacter(Character& character);

private:

	MapDivider* sections;
	const Section* const* activeSection;

	GraphicsStorage& storage;

	// Instanced
	const std::vector<InstancedObject*>& enviormentInstanced;

	// Sampler
	ID3D11SamplerState* sampler;  // << temporary

	Camera* activeCamera;

};