#pragma once
#include "Scene.h"

class MainMenu : public Scene
{
public:
	MainMenu(UIRenderer& uiRender, ObjectRender& objRender, DecalShadow& decalShadow, TempMeshStorage& meshStorage);

	// Inherited via Scene
	virtual void Load() override;
	virtual void Shutdown() override;
	virtual SceneState Update() override;
	virtual void Render() override;

private:
	PSprite startButton;//*

};