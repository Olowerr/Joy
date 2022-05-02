#pragma once
#include "ObjectRender.h"
#include "DecalShadow.h"
#include "UIRender.h"

enum struct SceneState { Unchanged, MainMenu, Easy, Test/*, Medium, Hard*/ };

__declspec(align(16))
class Scene
{
public:
	Scene(UIRenderer& uiRender, ObjectRender& objRender, DecalShadow& decalShadow, TempMeshStorage& meshStorage)
		:uiRender(uiRender), objRender(objRender), meshStorage(meshStorage), decalShadow(decalShadow)
	{

	}
	virtual ~Scene() { }

	virtual void Shutdown() = 0;
	virtual SceneState Update() = 0;
	virtual void Render() = 0;

protected:
	UIRenderer& uiRender;
	ObjectRender& objRender;
	TempMeshStorage& meshStorage;
	DecalShadow& decalShadow;

	// Overloaded operators
public:
	void* operator new(size_t i)
	{
		return _aligned_malloc(i, 16);
	}
	void operator delete(void* ptr)
	{
		_aligned_free(ptr);
	}
};

