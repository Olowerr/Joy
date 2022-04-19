#include "playground.h"

testScene::testScene(UIRenderer& uiRender, ObjectRender& objRender, TempMeshStorage& meshStorage)
    :Scene(uiRender, objRender, meshStorage), test(nullptr), collTest(nullptr)
{
}

void testScene::Load()
{
    meshStorage.LoadAll();
    test = new Character(meshStorage.GetMesh(1));
    collTest = new Character(meshStorage.GetMesh(2));
    test->SetPosition(3.0f, 0.0f, 3.0f);
    objRender.AddObject(test);
    objRender.AddObject(collTest);
}

void testScene::Shutdown()
{
    objRender.Clear();

    meshStorage.UnLoadAll();
    test->Shutdown();
    collTest->Shutdown();
    delete collTest;
    delete test;
}

SceneState testScene::Update()
{
    test->move();
    test->JumpAndBoost();
    test->respawn();
    if (coll.hitItem(test, collTest))
        test->setSpeedZero();
    coll.collided(test, collTest);

    return SceneState::Unchanged;
}

void testScene::Render()
{
    objRender.DrawAll();
}
