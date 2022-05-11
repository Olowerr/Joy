#pragma once
#include "Scene.h"
#include "Skybox.h"

class testScene : public Scene
{
public:
	testScene(UIRenderer& uiRender, ObjectRender& objRender, DecalShadow& decalShadow, TempMeshStorage& meshStorage);
	~testScene() { }

	// Inherited via Scene
	virtual void Shutdown() override;
	virtual SceneState Update() override;
	virtual void Render() override;

private:
	InstancedObject tast;

	Character joy;
	std::vector<Object> sceneObjects;

	HLight hLight;
	Camera* activeCamera;
	FreelookCamera freeCamera;
	CharacterCamera joyCamera;

	std::vector<Collision> coll;

	MapDivider divider;

	Skybox sky;
};