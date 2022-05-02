#include "EasyLevel.h"

EasyLevel::EasyLevel(UIRenderer& uiRender, ObjectRender& objRender, DecalShadow& decalShadow, TempMeshStorage& meshStorage)
	:Scene(uiRender, objRender, decalShadow, meshStorage)
{

}

void EasyLevel::Load()
{
	meshStorage.LoadAll();

	objects.reserve(66);
	objects.emplace_back(meshStorage.GetMesh(0), true);

	typedef DirectX::XMFLOAT3 F3;
	Object test[2] =
	{
		{meshStorage.GetMesh(0), true, F3(0.f, 0.f, 5.f)},
		{meshStorage.GetMesh(0), true, F3(0.f, 0.f, -5.f)}
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