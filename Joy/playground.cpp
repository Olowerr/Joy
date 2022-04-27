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
    joyCamera = new CharacterCamera(*test);
    objRender.AddObject(ground);
    objRender.AddObject(test);
    objRender.AddObject(bg);
    
    test->Translate(0, 0.5f, 0);
    ground->SetPosition(0.0f, -2.0f, 0.0f);
    bg->SetPosition(-1.f, 1, -5);


    objRender.AddObject(collTest);

    HLight hLight(objRender);
    Object* elgato[2] = { ground, bg };
    hLight.GenerateLightMaps(elgato, 2);
    hLight.Shutdown();

    fCamera = new FreelookCamera();
    activeCamera = joyCamera;
    objRender.SetActiveCamera(activeCamera);
}

void testScene::Shutdown()
{
    objRender.Clear();
    meshStorage.UnLoadAll();

    ground->Shutdown();
    delete ground;
    
    joyCamera->Shutdown();
    delete joyCamera;

    collTest->Shutdown();
    delete collTest;

    test->Shutdown();
    delete test;
    
    bg->Shutdown();
    delete bg;

    fCamera->Shutdown();
    delete fCamera;
}

SceneState testScene::Update()
{
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

    //ground->SetPosition(0, -2, 0);

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
