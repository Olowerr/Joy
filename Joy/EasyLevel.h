#pragma once
#include "Scene.h"

class EasyLevel : public Scene
{
public:
	EasyLevel(UIRenderer& uiRender, ObjectRender& objRender, TempMeshStorage& meshStorage);

	// Inherited via Scene
	virtual void Load() override;
	virtual void Shutdown() override;

	virtual SceneState Update() override;
	virtual void Render() override;


private:
	std::vector<Object> objects;

};