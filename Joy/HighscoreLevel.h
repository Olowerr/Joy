#pragma once
#include "Scene.h"

class HighscoreLevel : public Scene
{
public:
	HighscoreLevel(UIRenderer& uiRender, ObjectRender& objRender, DecalShadow& decalShadow, TempMeshStorage& meshStorage);
	~HighscoreLevel()  override { }

	// Inherited via Scene
	virtual void Shutdown() override;
	virtual SceneState Update() override;
	virtual void Render() override;

private:

	std::vector<Object> sceneObjects;
	Character joy;
	Object* ground1;
	Object* portal;
	Object* wall1;
	Object* wall2;
	Object* wall3;
	Object* highscore;
	Object* frame;

	Collision coll1;
	Collision coll2;
	Collision coll3;
	Collision coll4;

	HLight hLight;
	Camera* activeCamera;
	FreelookCamera freeCamera;
	CharacterCamera joyCamera;

	MapDivider divider;
};