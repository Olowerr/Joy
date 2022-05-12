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

    typedef DirectX::XMFLOAT3 F3;
    sceneObjects.reserve(10);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(6), true, F3(0.f, 1.f, 0.f));
    sceneObjects.emplace_back(meshStorage.GetObjMesh(6), true, F3(1.f, 3.f, -1.f));
    sceneObjects.emplace_back(meshStorage.GetObjMesh(1), false, F3(3.f, 1.f, 1.f));
    sceneObjects.emplace_back(meshStorage.GetObjMesh(1), false, F3(6.f, 1.f, 40.f));
    sceneObjects.emplace_back(meshStorage.GetObjMesh(2), true, F3(0.f, -2.f, 0.f));

    cube = &sceneObjects[0];
    ground = &sceneObjects[4];
    collTest = &sceneObjects[1];
    
    joy.SetPosition(0.f, 3.f, 0.f);

    objRender.SetActiveCamera(activeCamera);
    decalShadow.SetActiveCamera(activeCamera);

    divider.CreateSections(2, 50.f, 15.f, 10.f);
    objRender.SetMapDivier(&divider);
    decalShadow.SetMapDivider(&divider);
    

    hLight.InitiateTools(divider);

    InstancedObject::CreateInstancedObjects(meshStorage, divider, hLight);
    hLight.GenerateLightMaps(divider);

    hLight.ShutdownTools();

    sky.init();
}

void testScene::Shutdown()
{
    sky.Shutdown();

    hLight.Shutdown();
 
    objRender.Clear();
    meshStorage.UnloadObjMeshes();

    Object::EmptyObjectLists();
    InstancedObject::DestroyInstancedObjects();

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

    sky.Draw(activeCamera);
    uiRender.Draw();
}
