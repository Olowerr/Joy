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
	std::vector<Object> sceneObjects;
	Character joy;

	HLight hLight;
	Camera* activeCamera;
	FreelookCamera freeCamera;
	CharacterCamera joyCamera;

	std::vector<Collision> coll;

	Sprite catButton;
	Text thomas;
	float time = 0.f;

	MapDivider divider;
};