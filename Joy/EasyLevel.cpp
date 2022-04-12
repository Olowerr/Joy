#include "EasyLevel.h"
#include "Pickup.h"

EasyLevel::EasyLevel(UIRenderer& uiRender, ObjectRender& objRender, TempMeshStorage& meshStorage)
	:Scene(uiRender, objRender, meshStorage)
{
	// Implement Object::Create() & Object::Destroy() (same as for Sprite) for dynamic allocation
	// Implement Object::Create() & Object::Destroy() (same as for Sprite) for dynamic allocation
	// Implement Object::Create() & Object::Destroy() (same as for Sprite) for dynamic allocation
	// Implement Object::Create() & Object::Destroy() (same as for Sprite) for dynamic allocation
	// Implement Object::Create() & Object::Destroy() (same as for Sprite) for dynamic allocation
	// Implement Object::Create() & Object::Destroy() (same as for Sprite) for dynamic allocation
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
	objRender.GiveInstancedObjects(test, 2);


}

void EasyLevel::Shutdown()
{
	meshStorage.UnLoadAll();

	objRender.Clear();

	for (Object& obj : objects)
		obj.Shutdown();
	objects.clear();
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