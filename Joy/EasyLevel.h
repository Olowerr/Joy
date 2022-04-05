#pragma once
#include "Scene.h"

class EasyLevel : public Scene
{
public:
	EasyLevel();

	// Inherited via Scene
	virtual void Load() override;
	virtual void Shutdown() override;

	virtual void Update() override;
	virtual void Render() override;


private:
	std::vector<Object> objects;


	MeshStorage storage;
	ObjectRender render;
};