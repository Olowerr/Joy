#pragma once
#include "Scene.h"
#include "Character.h"
#include "CharacterCamera.h"
#include "Collision.h"
#include "FreelookCamera.h"
#include "LightHandler.h"
#include "MapSections.h"

#include "ImguiSetup.h"
#include "imgui\imgui.h"
#include "imgui\imgui_impl_win32.h"
#include "imgui\imgui_impl_dx11.h"

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

	MapDivider divider;
};