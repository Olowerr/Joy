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

  //  bg->Scale(2);

    viewAndProj = camera->GetViewAndProj();

    Backend::CreateDynamicCBuffer(&camCb, &viewAndProj, 64);
    Backend::UpdateBuffer(camCb, &viewAndProj, 64);
    devContext->VSSetConstantBuffers(1, 1, &camCb); 

    HLight hLight(objRender);
    Object* elgato[2] = { ground, bg };
    hLight.GenerateLightMaps(elgato, 2);
    hLight.Shutdown();

    fCamera = new FreelookCamera();
    activeCamera = joyCamera;
    objRender.SetActiveCamera(activeCamera);
    objRender.CreateCharacterDecal(joy);
    
    devContext->PSSetConstantBuffers(0, 1, objRender.getDecalBuffer());
    objRender.AddObject(collTest);
}

void testScene::Shutdown()
{
    objRender.Clear();

    joy->Shutdown();
    bg->Shutdown();
    ground->Shutdown();
    collTest->Shutdown();

    delete camera;
    collTest->Shutdown();
    delete joy;
    delete bg;
}

SceneState testScene::Update()
    if (Backend::GetKeyboard().KeyReleased(DIK_R))
    {
        activeCamera = fCamera;
        objRender.SetActiveCamera(activeCamera);
    }
    else if (Backend::GetKeyboard().KeyReleased(DIK_T))
    {
        activeCamera = joyCamera;
        objRender.SetActiveCamera(activeCamera);
    }
    activeCamera->UpdateCam();
    activeCamera->SetView();
    if (activeCamera == fCamera)
        return SceneState::Unchanged;
    Backend::UpdateBuffer(camCb, &viewAndProj, 64);
    //Camera functions
    camera->UpdateCam();
    camera->SetView();

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
    test->respawn();

    //Decal
    objRender.UpdateCharacterDecal(joy);


    return SceneState::Unchanged;
}

void testScene::Render()
{
    objRender.DrawAll();
}
