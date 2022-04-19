#pragma once
#include "Scene.h"
#include "Character.h"
#include "Collision.h"
class testScene : public Scene
{
public:
	testScene(UIRenderer& uiRender, ObjectRender& objRender, TempMeshStorage& meshStorage);
	// Inherited via Scene
	virtual void Load() override;

	virtual void Shutdown() override;


	virtual SceneState Update() override;


	virtual void Render() override;


	Character* test;
	Character* collTest;
	Collision coll;

};