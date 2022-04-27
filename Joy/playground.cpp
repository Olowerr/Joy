#include "playground.h"

testScene::testScene(UIRenderer& uiRender, ObjectRender& objRender, TempMeshStorage& meshStorage)
    :Scene(uiRender, objRender, meshStorage), joy(nullptr), bg(nullptr), collTest(nullptr), ground(nullptr)

{
}

void testScene::Load()
{
    ID3D11DeviceContext* devContext = Backend::GetDeviceContext();

    meshStorage.LoadAll();

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
}

void testScene::Shutdown()
{
    objRender.Clear();
    meshStorage.UnLoadAll();

    joy->Shutdown();
    ground->Shutdown();
    collTest->Shutdown();

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

    //Collision
    joy->SetCanJump(false);
    joy->SetCollidedY(coll.getCollidedY());
    if (coll.HitObject(joy, collTest))
        joy->SetSpeedZero();
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
}
