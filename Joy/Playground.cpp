#include "playground.h"

<<<<<<< HEAD
testScene::testScene(UIRenderer& uiRender, ObjectRender& objRender, DecalShadow& decalShadow, TempMeshStorage& meshStorage)
    :Scene(uiRender, objRender, decalShadow, meshStorage), joy(nullptr), bg(nullptr), collTest(nullptr), ground(nullptr)

=======
testScene::testScene(UIRenderer& uiRender, ObjectRender& objRender, TempMeshStorage& meshStorage)
    :Scene(uiRender, objRender, meshStorage), joy(nullptr), gatoKubo(nullptr), collTest(nullptr), ground(nullptr)
    , activeCamera(nullptr), freeCamera(nullptr), joyCamera(nullptr)
>>>>>>> master
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
<<<<<<< HEAD
    camera = new CharacterCamera(*joy);
    objRender.AddObject(joy);
    objRender.AddObject(ground);
    objRender.AddObject(bg);
=======
    gatoKubo = new Object(meshStorage.GetMesh(1));

    //Camera recives which object to look at
    joyCamera = new CharacterCamera(*joy);


    objRender.AddObject(ground);
    objRender.AddObject(joy);
    objRender.AddObject(gatoKubo);

>>>>>>> master
    ground->SetPosition(0.0f, -2.0f, 0.0f);
    collTest->SetPosition(-20.0f, 0.0f, 0.0f);
    gatoKubo->SetPosition(1.f, 0.5f, 1.f);

<<<<<<< HEAD
  //  bg->Scale(2);

    // Create decal buffers. Camera and character pos as dynamic constant buffers.
    decalShadow.CreateCharacterDecal(joy);
    decalShadow.CreateDecalDepthCam(joy);

    // Add Comment
    viewAndProj = camera->GetViewAndProj();
    Backend::CreateDynamicCBuffer(&camCb, &viewAndProj, 64);

=======
    HLight hLight(objRender);
    Object* elgato[2] = { ground, gatoKubo };
    hLight.GenerateLightMaps(elgato, 2);
    hLight.Shutdown();

    freeCamera = new FreelookCamera();
    activeCamera = joyCamera;
    objRender.SetActiveCamera(activeCamera);
    objRender.CreateCharacterDecal(joy);
    
    devContext->PSSetConstantBuffers(0, 1, objRender.getDecalBuffer());
>>>>>>> master
    objRender.AddObject(collTest);
}

void testScene::Shutdown()
{
    objRender.Clear();
    meshStorage.UnLoadAll();

    joy->Shutdown();
    gatoKubo->Shutdown();
    ground->Shutdown();
    collTest->Shutdown();

    freeCamera->Shutdown();
    joyCamera->Shutdown();

    delete freeCamera;
    delete joyCamera;

    delete joy;
    delete gatoKubo;
    delete collTest;
    delete ground;
}

SceneState testScene::Update()
{
    ID3D11DeviceContext* devContext = Backend::GetDeviceContext();
    viewAndProj = camera->GetViewAndProj();
    devContext->VSSetConstantBuffers(1, 1, &camCb);
    Backend::UpdateBuffer(camCb, &viewAndProj, 64);

    if (Backend::GetKeyboard().KeyReleased(DIK_R))
    {
        activeCamera = freeCamera;
        objRender.SetActiveCamera(activeCamera);
    }
    else if (Backend::GetKeyboard().KeyReleased(DIK_T))
    {
        activeCamera = joyCamera;
        objRender.SetActiveCamera(activeCamera);
    }
    activeCamera->UpdateCam();
    activeCamera->SetView();

    if (activeCamera == freeCamera)
        return SceneState::Unchanged;

    //Camera functions
    activeCamera->UpdateCam();
    activeCamera->SetView();

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

    joy->move();
    joy->respawn();

    // Update Character and Camera pos for the buffers.
    decalShadow.UpdateCharacterDecal(joy);
    decalShadow.UpdateDecalDepthCam(joy);

    joy->Move();
    joy->Respawn();
    //test->respawn();

    //Decal
    objRender.UpdateCharacterDecal(joy);

    return SceneState::Unchanged;
}

void testScene::Render()
{
    objRender.DrawAll();
}
