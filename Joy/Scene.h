#pragma once
#include "ObjectRender.h"
#include "UIRender.h"

enum struct SceneState { Unchanged, MainMenu, Easy, Test/*, Medium, Hard*/ };

class Scene
{
public:
	Scene(UIRenderer& uiRender, ObjectRender& objRender, TempMeshStorage& meshStorage)
		:uiRender(uiRender), objRender(objRender), meshStorage(meshStorage)
	{

	}

	virtual void Load() = 0;
	virtual void Shutdown() = 0;

	virtual SceneState Update() = 0;
	virtual void Render() = 0;

protected:
	UIRenderer& uiRender;
	ObjectRender& objRender;
	TempMeshStorage& meshStorage;
};

