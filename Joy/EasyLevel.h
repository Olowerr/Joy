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
	Object* portal;
	Object* tree1;
	Object* tree2;

	HLight hLight;
	Camera* activeCamera;
	FreelookCamera freeCamera;
	CharacterCamera joyCamera;

	Collision coll1;
	Collision coll2;
	Collision coll3;
	Collision coll4;
	Collision coll5;
	Collision coll6;
	Collision coll7;
	Collision coll8;
	Collision coll9;
	Collision coll10;

	Sprite pickUpUI;
	Text thomas;
	float time = 0.f;

	MapDivider divider;
	std::unique_ptr<DirectX::AudioEngine> audEngine;
	std::unique_ptr<DirectX::SoundEffect> soundEffect;
	std::unique_ptr<DirectX::SoundEffectInstance> effect;
};