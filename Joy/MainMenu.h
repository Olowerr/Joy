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
	Object* ground1;
	Object* portal1;
	Object* portal2;
	Object* wall1;
	Object* wall2;
	Object* wall3;

	Collision coll1;
	Collision coll2;
	Collision coll3;
	Collision coll4;

	HLight hLight;
	Camera* activeCamera;
	FreelookCamera freeCamera;
	CharacterCamera joyCamera;

	MapDivider divider;

	Sprite startButton;
};