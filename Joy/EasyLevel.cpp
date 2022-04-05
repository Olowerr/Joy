#include "EasyLevel.h"

EasyLevel::EasyLevel()
	:Scene(SceneState::Easy)
{
}

void EasyLevel::Load()
{
	storage.LoadAll();

	render.initiate();

	objects.reserve(66);
	objects.emplace_back(storage.GetMesh(0));

	render.Add(&objects[0]);
}

void EasyLevel::Shutdown()
{
	render.Shutdown();
	storage.Shutdown();

	for (Object& obj : objects)
	{
		obj.Shutdown();
	}
}

void EasyLevel::Update()
{
	objects[0].Rotate(0.f, 2.f * Backend::GetDeltaTime(), 0.f);
}

void EasyLevel::Render()
{
	render.DrawAll();
}