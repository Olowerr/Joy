#pragma once
#include "Scene.h"
#include "HighscoreClass.h"
#include "Pickup.h"

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

	HighscoreClass m_highscore;
	float time = 0.f;

	Character joy;
	std::vector<Object> sceneObjects;
	std::vector<Collision> collisions;

	Pickup pickups;

	HLight hLight;
	Camera* activeCamera;
	FreelookCamera freeCamera;
	CharacterCamera joyCamera;

	Sprite loadingScreen;
	Sprite barUI;
	Sprite arrow;
	Text thomas;
	Text timeReduction;
	float timeReducMover;
	float timeRedSpeedUpp;
	float dt;
	

	MapDivider divider;
	Skybox sky;


	void PlaceObjects();
	void PlacePickups(TempMeshStorage &meshStorage);
};