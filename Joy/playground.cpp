#include "playground.h"

testScene::testScene(UIRenderer& uiRender, ObjectRender& objRender, TempMeshStorage& meshStorage)
    :Scene(uiRender, objRender, meshStorage), test(nullptr), bg(nullptr), collTest(nullptr)

{
}

void testScene::Load()
{
    ID3D11DeviceContext* devContext = Backend::GetDeviceContext();

    meshStorage.LoadAll();
    test = new Character(meshStorage.GetMesh(1));
    collTest = new Character(meshStorage.GetMesh(2));
    test->SetPosition(3.0f, 0.0f, 3.0f);
    bg = new Object(meshStorage.GetMesh(3));
    camera = new CharacterCamera(*test);

    objRender.AddObject(test);
    objRender.AddObject(bg);
    test->Translate(1.f, 0, 0);

    bg->SetPosition(-1.f, 1, -5);
    //bg->SetScale(1.f / 23.f);

  //  bg->Scale(2);

    viewAndProj = camera->GetViewAndProj();

    Backend::CreateDynamicCBuffer(&camCb, &viewAndProj, 64);
    Backend::UpdateBuffer(camCb, &viewAndProj, 64);
    devContext->VSSetConstantBuffers(1, 1, &camCb); 
    objRender.AddObject(collTest);
}

void testScene::Shutdown()
{
    objRender.Clear();
    meshStorage.UnLoadAll();
    test->Shutdown();
    bg->Shutdown();
    delete camera;
    collTest->Shutdown();
    delete collTest;
    delete test;
    delete bg;
}

SceneState testScene::Update()
{


    ID3D11DeviceContext* devContext = Backend::GetDeviceContext();
    viewAndProj = camera->GetViewAndProj();
    devContext->VSSetConstantBuffers(1, 1, &camCb);
    Backend::UpdateBuffer(camCb, &viewAndProj, 64);

    camera->UpdateCam();
    camera->SetView();
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
