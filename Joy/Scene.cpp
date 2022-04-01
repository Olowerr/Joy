#include "Scene.h"


Scene::Scene(SceneState state)
	:state(state)
{
}

MainScene::MainScene()
	:Scene(SceneState::MainMenu)
{
}

void MainScene::Load()
{
	storage.Load();

	render.initiate();

	objects.reserve(66);
	objects.emplace_back(storage.GetMesh(0));

	render.Add(&objects[0]);
}

void MainScene::Shutdown()
{
	render.Shutdown();
	storage.Shutdown();

	for (Object& obj : objects)
	{
		obj.Shutdown();
	}
}

void MainScene::Update()
{
	objects[0].Rotate(0.f, 2.f * Backend::Get().GetDeltaTime(), 0.f);
}

void MainScene::Render()
{
	render.DrawAll();
}