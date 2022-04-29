#pragma once
#include "Scene.h"
#include "Character.h"
#include "CharacterCamera.h"
#include "Collision.h"
#include "FreelookCamera.h"
#include "LightHandler.h"

class testScene : public Scene
{
public:
	testScene(UIRenderer& uiRender, ObjectRender& objRender, DecalShadow& decalShadow, TempMeshStorage& meshStorage);
	
	// Inherited via Scene
	virtual void Load() override;
	virtual void Shutdown() override;
	virtual SceneState Update() override;
	virtual void Render() override;

private:

	Character* joy;
	Object* ground;
	Object* gatoKubo;

	Camera* activeCamera;
	FreelookCamera* freeCamera;
	CharacterCamera* joyCamera;

	Object* collTest;
	Collision coll;
};