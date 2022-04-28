#pragma once
#include "Scene.h"
#include "Character.h"
#include "CharacterCamera.h"
#include "Collision.h"
#include "LightHandler.h"
#include "FreelookCamera.h"

class testScene : public Scene
{
public:
	testScene(UIRenderer& uiRender, ObjectRender& objRender, TempMeshStorage& meshStorage);
	
	// Inherited via Scene
	virtual void Load() override;
	virtual void Shutdown() override;
	virtual SceneState Update() override;
	virtual void Render() override;

private:

	Character* joy;
	Object* ground;
	Object* bg;

	Camera* activeCamera;
	FreelookCamera* fCamera;
	CharacterCamera* joyCamera;
	ID3D11Buffer* camCb;
	DirectX::XMFLOAT4X4 viewAndProj;

	Object* collTest;
	Collision coll;
};