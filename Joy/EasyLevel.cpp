#include "EasyLevel.h"

EasyLevel::EasyLevel(UIRenderer& uiRender, ObjectRender& objRender, MeshStorage& meshStorage)
	:Scene(uiRender, objRender, meshStorage)
{
}

void EasyLevel::Load()
{

	objects.reserve(66);
	objects.emplace_back(meshStorage.GetMesh(0));

	objRender.Add(&objects[0]);
}

void EasyLevel::Shutdown()
{
	for (Object& obj : objects)
	{
		obj.Shutdown();
	}
}

SceneState EasyLevel::Update()
{
	objects[0].Rotate(0.f, 2.f * Backend::GetDeltaTime(), 0.f);

	return SceneState::Unchanged;
}

void EasyLevel::Render()
{
	objRender.DrawAll();
}