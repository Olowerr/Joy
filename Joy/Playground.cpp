#include "playground.h"

testScene::testScene(UIRenderer& uiRender, ObjectRender& objRender, TempMeshStorage& meshStorage)
    :Scene(uiRender, objRender, meshStorage), joy(nullptr), bg(nullptr), collTest(nullptr), ground(nullptr)

{
}

void testScene::Load()
{
    ID3D11DeviceContext* devContext = Backend::GetDeviceContext();

    meshStorage.LoadAll();
    joy = new Character(meshStorage.GetMesh(0));
    collTest = new Object(meshStorage.GetMesh(1));
    //test->SetPosition(0.0f, 0.2f, .0f);
    bg = new Object(meshStorage.GetMesh(1));
    ground = new Object(meshStorage.GetMesh(2));
    camera = new CharacterCamera(*joy);
    objRender.AddObject(ground);
    objRender.AddObject(joy);
    objRender.AddObject(bg);
    ground->SetPosition(0.0f, -2.0f, 0.0f);
    bg->SetPosition(-10.0f, 1.0f, -5.0f);
    collTest->SetPosition(-20.0f, 0.0f, 0.0f);
    //bg->SetScale(1.f / 23.f);

  //  bg->Scale(2);

    viewAndProj = camera->GetViewAndProj();

    Backend::CreateDynamicCBuffer(&camCb, &viewAndProj, 64);
    Backend::UpdateBuffer(camCb, &viewAndProj, 64);
    devContext->VSSetConstantBuffers(1, 1, &camCb); 
    objRender.AddObject(collTest);

    objRender.CreateCharacterDecal(joy);
    devContext->PSSetConstantBuffers(0, 1, objRender.getDecalBuffer());
}

void testScene::Shutdown()
{
    objRender.Clear();
    meshStorage.UnLoadAll();
    joy->Shutdown();
    bg->Shutdown();
    ground->Shutdown();
    delete ground;
    delete camera;
    collTest->Shutdown();
    delete collTest;
    delete joy;
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



    joy->SetCanJump(false);
    joy->setCollidedY(coll.getCollidedY());
    if (coll.hitObject(joy, collTest))
        joy->setSpeedZero();
    if (coll.hitObject(joy, ground))
        joy->SetCanJump(coll.GetStopFall());
    if (coll.hitObject(joy, collTest))
        joy->SetCanJump(coll.GetStopFall());
    
    coll.collided(joy, collTest);







    joy->Jump();
    joy->Move();
    joy->Respawn();
    objRender.UpdateCharacterDecal(joy);

    //Collision
    

    return SceneState::Unchanged;
}

void testScene::Render()
{
    objRender.DrawAll();
}
