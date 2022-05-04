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
	Object* ground;
	Object* collTest;
	Object* cube;

	HLight hLight;
	Camera* activeCamera;
	FreelookCamera freeCamera;
	CharacterCamera joyCamera;

	Collision coll;
	Collision coll2;
	Collision coll3;

	MapDivider divider;
};