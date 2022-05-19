#pragma once
#include "Scene.h"
#include "HighscoreClass.h"

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

	Character joy;
	std::vector<Object> sceneObjects;
	std::vector<Collision> collisions;

	HLight hLight;
	Camera* activeCamera;
	FreelookCamera freeCamera;
	CharacterCamera joyCamera;

	Sprite loadingScreen;
	Sprite pickUpUI;
	Text thomas;
	float time = 0.f;

	MapDivider divider;
	Skybox sky;


	void PlaceObjects();
};