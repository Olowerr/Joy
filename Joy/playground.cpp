#include "playground.h"

testScene::testScene(UIRenderer& uiRender, ObjectRender& objRender, TempMeshStorage& meshStorage)
    :Scene(uiRender, objRender, meshStorage), test(nullptr), bg(nullptr), collTest(nullptr), ground(nullptr)

{
}

void testScene::Load()
{
    ID3D11DeviceContext* devContext = Backend::GetDeviceContext();

    meshStorage.LoadAll();
    test = new Character(meshStorage.GetMesh(1));
    collTest = new Character(meshStorage.GetMesh(2));
    //test->SetPosition(0.0f, 0.2f, .0f);
    bg = new Object(meshStorage.GetMesh(2));
    ground = new Object(meshStorage.GetMesh(3));
    camera = new CharacterCamera(*test);
    objRender.AddObject(ground);
    objRender.AddObject(test);
    objRender.AddObject(bg);
    
    test->Translate(0, 0.5f, 0);
    ground->SetPosition(0.0f, -2.0f, 0.0f);
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
    ground->Shutdown();
    delete ground;
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

    //ground->SetPosition(0, -2, 0);
    //Camera
    camera->UpdateCam();
    camera->SetView();
    //test->SetStopMovement(coll.GetDontStopMovement());
    test->SetCanJump(false);
    test->setCollidedY(coll.getCollidedY());
    if (coll.hitObject(test, collTest))
        test->setSpeedZero();
    if (coll.hitObject(test, ground))
        test->SetCanJump(coll.GetStopFall());
    if (coll.hitObject(test, collTest))
        test->SetCanJump(coll.GetStopFall());
    
    coll.collided(test, collTest);
    //coll.collided(test, ground);
    //Test Character(cube)
    test->Jump();
    test->move();
    test->respawn();

    //Collision
    

    return SceneState::Unchanged;
}

void testScene::Render()
{
    objRender.DrawAll();
}
