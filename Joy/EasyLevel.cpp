#include "EasyLevel.h"


EasyLevel::EasyLevel(UIRenderer& uiRender, ObjectRender& objRender, TempMeshStorage& meshStorage)
	:Scene(uiRender, objRender, meshStorage), pickups(nullptr)
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

	// TODO: DoubleCheck
	pickups = new Pickup(meshStorage, 2);
	pickups->AddObject(0.0f, 0.0f, 0.0f);
	pickups->CreateSRV_CreateMatrixCB();

	// TODO: Add instance vector/Array.


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
	// TODO: Check this
	delete this->pickups;
	pickups->ShutDown();

	meshStorage.UnLoadAll();

	objRender.Clear();

	for (Object& obj : objects)
		obj.Shutdown();
	objects.clear();


}

SceneState EasyLevel::Update()
{
	objects[0].Rotate(0.f, 2.f * Backend::GetDeltaTime(), 0.f);

	// TODO : Update main objects.
	pickups->UpdateMatrices();

	return SceneState::Unchanged;
}

void EasyLevel::Render()
{
	objRender.DrawAll();
	pickups->DrawPickupInstances();
	// TODO : Enable it to draw all pickups.
}