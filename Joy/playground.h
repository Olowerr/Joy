#pragma once
#include "Scene.h"

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
	Character joy;
	std::vector<Object> sceneObjects;

	std::vector<Collision> coll;

	std::vector<Collision> collisions;


	HLight hLight;
	Camera* activeCamera;
	FreelookCamera freeCamera;
	CharacterCamera joyCamera;

	MapDivider divider;
	Skybox sky;
};