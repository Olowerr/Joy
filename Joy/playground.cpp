#include "playground.h"

testScene::testScene(UIRenderer& uiRender, ObjectRender& objRender, TempMeshStorage& meshStorage)
    :Scene(uiRender, objRender, meshStorage), test(nullptr)
{
}

void testScene::Load()
{
    meshStorage.LoadAll();
    test = new Character(meshStorage.GetMesh(1));
    objRender.AddObject(test);
}

void testScene::Shutdown()
{
    objRender.Clear();

    meshStorage.UnLoadAll();
    test->Shutdown();
    delete test;
}

SceneState testScene::Update()
{
    test->move();
    test->JumpAndBoost();
    return SceneState::Unchanged;
}

void testScene::Render()
{
    objRender.DrawAll();
}
