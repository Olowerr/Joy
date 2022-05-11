#include "playground.h"

testScene::testScene(UIRenderer& uiRender, ObjectRender& objRender, DecalShadow& decalShadow, TempMeshStorage& meshStorage)
    :Scene(uiRender, objRender, decalShadow, meshStorage)
    // Joy should always be first in the array from mesh storage
    , joy(meshStorage.GetObjMesh(0))
    , joyCamera(joy)
    , divider(joy)
    , activeCamera(&joyCamera)
{
    meshStorage.LoadAllObj();

    joy.CheckBB();

    sceneObjects.reserve(20);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(6), true);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(6), true);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(6), true);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(2), true);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(10), true);
    
    cube = &sceneObjects[0];
    ground = &sceneObjects[3];
    collTest = &sceneObjects[1];
    
    joy.SetPosition(0.f, 3.f, 0.f);
    ground->SetPosition(0.f, -2.0f, 0.f);
    sceneObjects[0].SetPosition(0.0f, 1.0f, 0.0f);
    sceneObjects[1].SetPosition(1.f, 3.f, -1.0f);
    sceneObjects[2].SetPosition(3.f, 1.0f, 1.f);
    sceneObjects[4].SetPosition(-3.f, 0.5f, -2.f);

    /*meshStorage.LoadMenuObjects();
    for (size_t i = 0; i < meshStorage.GetMeshCount(); i++)
    {
        sceneObjects.emplace_back(meshStorage.GetMesh(i), true);
        sceneObjects.back().SetPosition(i * 5.f, (i + 1) * 5.f, 0.f);
    }*/

    objRender.SetActiveCamera(activeCamera);
    decalShadow.SetActiveCamera(activeCamera);

    divider.CreateSections(1, 50.f, 15.f, 10.f);
    objRender.SetMapDivier(&divider);
    decalShadow.SetMapDivider(&divider);
    
    hLight.InitiateTools(divider);
    hLight.GenerateLightMaps(divider);

    tast.AddObject(&sceneObjects[0]);
    tast.AddObject(&sceneObjects[1]);
    tast.AddObject(&sceneObjects[2]);
    hLight.GenerateLightMapsInstanced(divider, tast);

    tast.Finalize();

    hLight.ShutdownTools();


    sky.init();

    meshStorage.UnloadDataBase();
}

void testScene::Shutdown()
{
    sky.Shutdown();

    hLight.Shutdown();
    tast.Shutdown();

    objRender.Clear();
    meshStorage.UnloadObjMeshes();
    meshStorage.UnloadMeshes();
    Object::EmptyObjectLists();
 
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
        decalShadow.SetActiveCamera(activeCamera);
    }
    else if (Backend::GetKeyboard().KeyReleased(DIK_T))
    {
        activeCamera = &joyCamera;
        objRender.SetActiveCamera(activeCamera);
        decalShadow.SetActiveCamera(activeCamera);
    }
    activeCamera->UpdateCam();
    activeCamera->SetView();

    if (activeCamera == &freeCamera)
        return SceneState::Unchanged;

    joy.Jump();
    joy.Move();
    joy.Respawn();
    

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
    uiRender.Draw();

    sky.Draw(activeCamera);
}
