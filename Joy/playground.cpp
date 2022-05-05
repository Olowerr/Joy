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
    sceneObjects[0].SetPosition(0.0f, 1.0f, 0.0f);
    sceneObjects[1].SetPosition(1.f, 3.f, -1.0f);
    sceneObjects[2].SetPosition(3.f, 1.0f, 1.f);

    objRender.SetActiveCamera(activeCamera);
    decalShadow.SetActiveCamera(activeCamera);

    divider.CreateSections(1, 15.f, 15.f, 10.f);
    objRender.SetMapDivier(&divider);
    decalShadow.SetMapDivider(&divider);

    tast.AddObject(&sceneObjects[0]);
    tast.AddObject(&sceneObjects[1]);
    tast.AddObject(&sceneObjects[2]);
    
    hLight.InitiateTools(divider);
    hLight.GenerateLightMaps(divider);
    hLight.GenerateLightMapsInstanced(divider, tast);
    tast.Finalize();

    hLight.ShutdownTools();
}

void testScene::Shutdown()
{
    hLight.Shutdown();
    tast.Shutdown();

    objRender.Clear();
    meshStorage.UnLoadAll();
   // Object::EmptyObjectLists();
 
    joy.Shutdown();
    for (Object& object : sceneObjects)
        object.Shutdown();

    freeCamera.Shutdown();
    joyCamera.Shutdown();

    divider.Shutdown();
}

SceneState testScene::Update()
{
    joy.Jump();
    joy.Move();
    joy.Respawn();
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


    if (coll.getCollidedY() || coll2.getCollidedY() || coll3.getCollidedY())
        joy.SetCanJump(true);
    else
        joy.SetCanJump(false);

    coll.collided(&joy, collTest);
    coll2.collided(&joy, cube);
    coll3.collided(&joy, ground);

    return SceneState::Unchanged;
}

void testScene::Render()
{
    objRender.DrawAll();
    decalShadow.DrawAll(joy.GetPosition());
    objRender.DrawCharacter(joy);
}
