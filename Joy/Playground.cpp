#include "playground.h"

testScene::testScene(UIRenderer& uiRender, ObjectRender& objRender, TempMeshStorage& meshStorage)
    :Scene(uiRender, objRender, meshStorage), joy(nullptr), gatoKubo(nullptr), collTest(nullptr), ground(nullptr)
    , activeCamera(nullptr), freeCamera(nullptr), joyCamera(nullptr)
{
}

void testScene::Load()
{
    ID3D11DeviceContext* devContext = Backend::GetDeviceContext();

    meshStorage.LoadAll();

    // Joy should always be first in the array from mesh storage
    joy = new Character(meshStorage.GetMesh(0)); 
    collTest = new Object(meshStorage.GetMesh(1), true);
    ground = new Object(meshStorage.GetMesh(2), true);
    gatoKubo = new Object(meshStorage.GetMesh(1), true);
    cube = new Object(meshStorage.GetMesh(1), true);

    //Camera recives which object to look at
    joyCamera = new CharacterCamera(*joy);

    objRender.AddObject(joy);
    objRender.AddObject(ground);
    objRender.AddObject(gatoKubo);

    cube->SetPosition(2.0f, 0.0f, 0.0f);
    ground->SetPosition(0.0f, -2.0f, 0.0f);
    collTest->SetPosition(2.0f, 0.0f, 0.0f);
    gatoKubo->SetPosition(1.f, 0.5f, 1.f);

    HLight hLight(objRender);
    Object* elgato[2] = { ground, gatoKubo };
    hLight.GenerateLightMaps(elgato, 2);
    hLight.Shutdown();

    freeCamera = new FreelookCamera();
    activeCamera = joyCamera;
    objRender.SetActiveCamera(activeCamera);

    objRender.AddObject(collTest);

    divider = new MapDivider(3, 15.f, 10.f, 10.f);
}

void testScene::Shutdown()
{
    objRender.Clear();
    meshStorage.UnLoadAll();
    joy->Shutdown();
    gatoKubo->Shutdown();
    ground->Shutdown();
    collTest->Shutdown();
    cube->Shutdown();

    freeCamera->Shutdown();
    joyCamera->Shutdown();

    delete freeCamera;
    delete joyCamera;

    delete joy;
    delete gatoKubo;
    delete collTest;
    delete ground;
    delete cube;
}

SceneState testScene::Update()
{
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
    joy->SetCollidedY(coll.getCollidedY());
    if(coll.getCollidedY())
        joy->SetSpeedZero();
    if (coll.HitObject(joy, cube))
        joy->SetSpeedZero();
    if (coll.HitObject(joy, ground))
    {
        joy->SetSpeedZero();
        joy->SetCanJump(coll.GetStopFall());
    }
        
    if (coll.HitObject(joy, collTest))
    {
        joy->SetSpeedZero();
    }
    joy->SetCanJump(coll.GetStopFall());
        
    coll.collided(joy, collTest);
    coll.collided(joy, cube);
    coll.collided(joy, ground);
    //Joy functions
    joy->Jump();
    joy->Move();
    joy->Respawn();

    return SceneState::Unchanged;
}

void testScene::Render()
{
    objRender.DrawAll();
}
