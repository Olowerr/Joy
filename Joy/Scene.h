#pragma once
#include "ObjectRender.h"

#include<vector>

enum struct SceneState { Unchanged, MainMenu, MainGame };

class Scene
{
public:
	Scene(SceneState state);

	virtual void Load() = 0;
	virtual void Shutdown() = 0;

	virtual void Update() = 0;
	virtual void Render() = 0;

private:
	SceneState state;
};

class MainScene : public Scene
{
public:
	MainScene();

	// Inherited via Scene
	virtual void Load() override;
	virtual void Shutdown() override;

	virtual void Update() override;
	virtual void Render() override;


private:
	std::vector<Object> objects;
	
	//temp
	MeshStorage storage;
	ObjectRender render;
	
};