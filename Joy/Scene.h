#pragma once
#include "ObjectRender.h"

enum struct SceneState { Unchanged, MainMenu, Easy, Medium, Hard };

class Scene
{
public:
	Scene(SceneState state)
		:state(state)
	{

	}

	virtual void Load() = 0;
	virtual void Shutdown() = 0;

	virtual void Update() = 0;
	virtual void Render() = 0;

private:
	const SceneState state;
};

