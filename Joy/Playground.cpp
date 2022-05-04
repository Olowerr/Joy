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
    collTest = new Object(meshStorage.GetMesh(1));
    ground = new Object(meshStorage.GetMesh(2));
    gatoKubo = new Object(meshStorage.GetMesh(1));
    cube = new Object(meshStorage.GetMesh(1));

    //Camera recives which object to look at
    joyCamera = new CharacterCamera(*joy);


    objRender.AddObject(ground);
    objRender.AddObject(joy);
    objRender.AddObject(gatoKubo);
    objRender.AddObject(cube);

    cube->SetPosition(-2.0f, 0.0f, 0.0f);
    ground->SetPosition(0.0f, -2.0f, 0.0f);
    collTest->SetPosition(2.0f, 0.0f, 0.0f);
    gatoKubo->SetPosition(1.f, 0.5f, 1.f);

    HLight hLight(objRender);
    Object* elgato[3] = { ground, gatoKubo, collTest };
    hLight.GenerateLightMaps(elgato, 3);
    hLight.Shutdown();

    freeCamera = new FreelookCamera();
    activeCamera = joyCamera;
    objRender.SetActiveCamera(activeCamera);
    objRender.CreateCharacterDecal(joy);
    
    devContext->PSSetConstantBuffers(0, 1, objRender.getDecalBuffer());
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
    joy->Jump();
    joy->Move();
    joy->Respawn();
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


    if (coll.getCollidedY() || coll2.getCollidedY() || coll3.getCollidedY())
        joy->SetCanJump(true);
    else
        joy->SetCanJump(false);
    coll.collided(joy, collTest);
    coll2.collided(joy, cube);
    coll3.collided(joy, ground);

    
    
    //Joy functions

    //test->respawn();

    //Decal
    objRender.UpdateCharacterDecal(joy);


    return SceneState::Unchanged;
}

void testScene::Render()
{
    objRender.DrawAll();
}
