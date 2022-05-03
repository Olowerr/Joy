#include "playground.h"

testScene::testScene(UIRenderer& uiRender, ObjectRender& objRender, DecalShadow& decalShadow, TempMeshStorage& meshStorage)
    :Scene(uiRender, objRender, decalShadow, meshStorage)
    // Joy should always be first in the array from mesh storage
    , joy(meshStorage.GetMesh(0))
    , joyCamera(joy)
    , divider(joy)
    , activeCamera(&joyCamera)
{
    meshStorage.LoadAll();

    joy.CheckBB();

    sceneObjects.reserve(10);
    sceneObjects.emplace_back(meshStorage.GetMesh(1), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(1), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(1), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(2), true);

    cube = &sceneObjects[0];
    ground = &sceneObjects[3];
    collTest = &sceneObjects[1];
    
    joy.SetPosition(0.f, 3.f, 0.f);
    ground->SetPosition(0.f, -2.0f, 0.f);
    sceneObjects[0].SetPosition(2.0f, 0.0f, 0.0f);
    sceneObjects[1].SetPosition(2.0f, 0.f, 0.0f);
    sceneObjects[2].SetPosition(1.f, 0.5f, 1.f);



    hLight.GenerateLightMaps();
    hLight.ShutdownTools();

    objRender.SetActiveCamera(activeCamera);
    decalShadow.SetActiveCamera(activeCamera);

    divider.CreateSections(1, 50.f, 50.f, 50.);
    objRender.SetMapDivier(&divider);
    decalShadow.SetMapDivider(&divider);
}

void testScene::Shutdown()
{
    hLight.Shutdown();

    decalShadow.Shutdown();
    objRender.Clear();
    meshStorage.UnLoadAll();
 
    joy.Shutdown();
    for (Object& object : sceneObjects)
        object.Shutdown();

    freeCamera.Shutdown();
    joyCamera.Shutdown();

    divider.Shutdown();
}

SceneState testScene::Update()
{
    if (Backend::GetKeyboard().KeyReleased(DIK_R))
    {
        activeCamera = &freeCamera;
        objRender.SetActiveCamera(activeCamera);
    }
    else if (Backend::GetKeyboard().KeyReleased(DIK_T))
    {
        activeCamera = &joyCamera;
        objRender.SetActiveCamera(activeCamera);
    }
    activeCamera->UpdateCam();
    activeCamera->SetView();

    if (activeCamera == &freeCamera)
        return SceneState::Unchanged;

    //Camera functions
    activeCamera->UpdateCam();
    activeCamera->SetView();

    //Collision
    joy.SetCollidedY(coll.getCollidedY());
    if (coll.getCollidedY())
        joy.SetSpeedZero();

   if (coll.HitObject(&joy, cube))
        joy.SetSpeedZero();

    if (coll.HitObject(&joy, ground))
    {
        joy.SetSpeedZero();
        joy.SetCanJump(coll.GetStopFall());
    }

    if (coll.HitObject(&joy, collTest))
    {
        joy.SetSpeedZero();
    }
    joy.SetCanJump(coll.GetStopFall());

    coll.collided(&joy, collTest);
    coll.collided(&joy, cube);
    coll.collided(&joy, ground);


    //Joy functions
    joy.Jump();
    joy.Move();
    joy.Respawn();

    return SceneState::Unchanged;
}

void testScene::Render()
{
    objRender.DrawAll();
    decalShadow.DrawAll(joy.GetPosition());
    objRender.DrawCharacter(joy);
}
