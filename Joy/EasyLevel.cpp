#include "EasyLevel.h"

EasyLevel::EasyLevel(UIRenderer& uiRender, ObjectRender& objRender, DecalShadow& decalShadow, TempMeshStorage& meshStorage)
	:Scene(uiRender, objRender, decalShadow, meshStorage)
{

}

void EasyLevel::Shutdown()
{

}

SceneState EasyLevel::Update()
{
	return SceneState::Unchanged;
}

void EasyLevel::Render()
{
	objRender.DrawAll();
}