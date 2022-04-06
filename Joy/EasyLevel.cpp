#include "EasyLevel.h"

EasyLevel::EasyLevel(UIRenderer& uiRender, ObjectRender& objRender, MeshStorage& meshStorage)
	:Scene(uiRender, objRender, meshStorage)
{
}

void EasyLevel::Load()
{
	meshStorage.LoadAll();

	objects.reserve(66);
	objects.emplace_back(meshStorage.GetMesh(0));

	objRender.AddObject(&objects[0]);

	typedef DirectX::XMFLOAT3 F3;
	Object test[2] =
	{
		{meshStorage.GetMesh(0), F3(0.f, 0.f, 5.f), F3(0.f, 0.f, 0.f), 1.f},
		{meshStorage.GetMesh(0), F3(0.f, 0.f, -5.f), F3(0.f, 0.f, 0.f), 1.f}
	};
	objRender.AddInstancedObjects(test, 2);

	test[0].Shutdown();
	test[1].Shutdown();
}

void EasyLevel::Shutdown()
{
	for (Object& obj : objects)
		obj.Shutdown();
	
	objRender.Clear();
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