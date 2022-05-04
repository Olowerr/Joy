#pragma once
#include "Scene.h"

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

	std::vector<Object> sceneObjects;
	Character joy;
	Object* ground;
	Object* ground1;
	Object* portal;
	Object* wall;
	Object* wall1;
	Object* wall2;

	HLight hLight;
	Camera* activeCamera;
	FreelookCamera freeCamera;
	CharacterCamera joyCamera;

	Collision coll;

	MapDivider divider;

	Sprite startButton;
};