#include "playground.h"

testScene::testScene(UIRenderer& uiRender, ObjectRender& objRender, TempMeshStorage& meshStorage)
    :Scene(uiRender, objRender, meshStorage), joy(nullptr), bg(nullptr), collTest(nullptr), ground(nullptr),
    pickups(nullptr), points(nullptr)

{
}

void testScene::Load()
{
    ID3D11DeviceContext* devContext = Backend::GetDeviceContext();
    meshStorage.LoadAll();

    pickups = new Pickup(meshStorage, 2);
    pickups->AddObject(2.0f, -2.0f, 2.0f);
    pickups->CreateSRV_CreateMatrixCB();

    points = new Points();
    points->setTrackedPickups(pickups);

        // Joy should always be first in the array from mesh storage
        joy = new Character(meshStorage.GetMesh(0)); 
        collTest = new Object(meshStorage.GetMesh(1));
        ground = new Object(meshStorage.GetMesh(2));

        //Camera recives wich object to look at
        camera = new CharacterCamera(*joy);

        objRender.AddObject(ground);
        objRender.AddObject(joy);

        ground->SetPosition(0.0f, -2.0f, 0.0f);
        collTest->SetPosition(-20.0f, 0.0f, 0.0f);

        viewAndProj = camera->GetViewAndProj();

        Backend::CreateDynamicCBuffer(&camCb, &viewAndProj, 64);
        Backend::UpdateBuffer(camCb, &viewAndProj, 64);
        devContext->VSSetConstantBuffers(1, 1, &camCb); 

        objRender.AddObject(collTest);
        objRender.CreateCharacterDecal(joy);
    
        devContext->PSSetConstantBuffers(0, 1, objRender.getDecalBuffer());

    points->StartTimer();
}

void testScene::Shutdown()
{
    points->StopTimer();

        objRender.Clear();
        meshStorage.UnLoadAll();

        joy->Shutdown();
        ground->Shutdown();
        collTest->Shutdown();

    pickups->ShutDown();
    delete pickups;
        delete ground;
        delete camera;
        delete collTest;
        delete joy;
}

SceneState testScene::Update()
{
    ID3D11DeviceContext* devContext = Backend::GetDeviceContext();
    viewAndProj = camera->GetViewAndProj();
    devContext->VSSetConstantBuffers(1, 1, &camCb);
    Backend::UpdateBuffer(camCb, &viewAndProj, 64);

    //Camera functions
    camera->UpdateCam();
    camera->SetView();
    
    pickups->UpdateMatrices();
    

    //Collision
    joy->SetCanJump(false);
    joy->setCollidedY(coll.getCollidedY());
    if (coll.HitObject(joy, collTest))
        joy->setSpeedZero();
    if (coll.HitObject(joy, ground))
        joy->SetCanJump(coll.GetStopFall());
    if (coll.HitObject(joy, collTest))
        joy->SetCanJump(coll.GetStopFall());
    
    coll.collided(joy, collTest);

    //Joy functions
    joy->Jump();
    joy->Move();
    joy->Respawn();

    //Decal
    objRender.UpdateCharacterDecal(joy);


    return SceneState::Unchanged;
}

void testScene::Render()
{
    objRender.DrawAll();
    pickups->DrawPickupInstances();
}
