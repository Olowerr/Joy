#pragma once
#include "Scene.h"

class EasyLevel : public Scene
{
public:
	EasyLevel(UIRenderer& uiRender, ObjectRender& objRender, DecalShadow& decalShadow, TempMeshStorage& meshStorage);
	~EasyLevel() override { }

	// Inherited via Scene
	virtual void Shutdown() override;

	virtual SceneState Update() override;
	virtual void Render() override;


private:
	Character joy;
	std::vector<Object> sceneObjects;
	std::vector<Collision> coll;

	HLight hLight;
	Camera* activeCamera;
	FreelookCamera freeCamera;
	CharacterCamera joyCamera;

	Sprite catButton;
	Text thomas;
	float time = 0.f;

	MapDivider divider;
	Skybox sky;
};