#pragma once
#include "Scene.h"

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
	//InstancedObject tast;
	std::vector<Object> sceneObjects;
	Character joy;
	Object* ground;
	Object* ground1;
	Object* ground2;
	Object* obstacle;
	Object* obstacle1;
	Object* obstacle2;
	Object* obstacle3;
	Object* obstacle4;

	HLight hLight;
	Camera* activeCamera;
	FreelookCamera freeCamera;
	CharacterCamera joyCamera;

	Collision coll;

	MapDivider divider;

	std::vector<Object> sceneObjects;
	Character joy;
	Object* ground;
	Object* ground1;
	Object* ground2;
	Object* obstacle;
	Object* obstacle1;
	Object* obstacle2;
	Object* obstacle3;
	Object* obstacle4;

	HLight hLight;
	Camera* activeCamera;
	FreelookCamera freeCamera;
	CharacterCamera joyCamera;

	Collision coll;

	MapDivider divider;

};