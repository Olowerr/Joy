#pragma once
#include "Scene.h"
#include "Character.h"
#include "CharacterCamera.h"
#include "Collision.h"
<<<<<<< HEAD
=======
#include "FreelookCamera.h"
#include "LightHandler.h"
>>>>>>> master

class testScene : public Scene
{
public:
<<<<<<< HEAD
	testScene(UIRenderer& uiRender, ObjectRender& objRender, DecalShadow& decalShadow, TempMeshStorage& meshStorage);
=======
	testScene(UIRenderer& uiRender, ObjectRender& objRender, TempMeshStorage& meshStorage);
	
>>>>>>> master
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