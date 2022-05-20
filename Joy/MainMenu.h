#pragma once
#include "Scene.h"
#include "HighscoreClass.h"
#include "PausMenu.h"

class MainMenu : public Scene
{
public:
	MainMenu(UIRenderer& uiRender, ObjectRender& objRender, DecalShadow& decalShadow, TempMeshStorage& meshStorage);
	~MainMenu()  override { }

	// Inherited via Scene
	virtual void Shutdown() override;
	virtual SceneState Update() override;
	virtual void Render() override;

private:
	Character joy;
	std::vector<Object> sceneObjects;
	std::vector<Collision> collisions;

	HLight hLight;
	Camera* activeCamera;
	FreelookCamera freeCamera;
	CharacterCamera joyCamera;

	Sprite loadingScreen;

	HighscoreClass highscore;

	MapDivider divider;
	Skybox sky;

	PausMenu pausMenu;
};