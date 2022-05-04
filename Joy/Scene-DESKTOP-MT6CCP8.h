#pragma once
#include "ObjectRender.h"
#include "DecalShadow.h"
#include "UIRender.h"

enum struct SceneState { Unchanged, MainMenu, Easy, Test/*, Medium, Hard*/ };

class Scene
{
public:
	Scene(UIRenderer& uiRender, ObjectRender& objRender, DecalShadow& decalShadow, TempMeshStorage& meshStorage)
		:uiRender(uiRender), objRender(objRender), decalShadow(decalShadow), meshStorage(meshStorage)
	{

	}

	virtual void Load() = 0;
	virtual void Shutdown() = 0;

	virtual SceneState Update() = 0;
	virtual void Render() = 0;

protected:
	UIRenderer& uiRender;
	ObjectRender& objRender;
	DecalShadow& decalShadow;
	TempMeshStorage& meshStorage;
};

