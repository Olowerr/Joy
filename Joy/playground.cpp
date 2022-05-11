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
    sceneObjects.emplace_back(meshStorage.GetMesh(6), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(6), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(6), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(2), true);
    for (int i = 0; i < (int)sceneObjects.size(); i++)
    {
        coll.emplace_back();
    }
    
    joy.SetPosition(0.f, 3.f, 0.f);
    sceneObjects[3].SetPosition(0.0f, -2.0f, 0.f);
    sceneObjects[0].SetPosition(0.0f, 1.0f, 0.0f);
    sceneObjects[1].SetPosition(1.0f, 3.0f, -1.0f);
    sceneObjects[2].SetPosition(3.0f, 1.0f, 1.0f);

    objRender.SetActiveCamera(activeCamera);
    decalShadow.SetActiveCamera(activeCamera);

    divider.CreateSections(1, 50.0f, 15.0f, 10.0f);
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
}

void testScene::Shutdown()
{
    sky.Shutdown();

    hLight.Shutdown();
    tast.Shutdown();

    objRender.Clear();
    meshStorage.UnLoadAll();
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
    
    //Camera functions
    activeCamera->UpdateCam();
    activeCamera->SetView();

    //Collision

    for (int i = 0; i < (int)sceneObjects.size(); i++)
    {
        if (coll.at(i).getCollidedY())
        {
            joy.SetCanJump(true);
            i = sceneObjects.size();
        }
        else
            joy.SetCanJump(false);
    }

    for (int i = 0; i < (int)sceneObjects.size(); i++)
    {
        for (int k = 0; k < sceneObjects.at(i).GetNumBboxes(); k++)
        {
            coll.at(i).collided(&joy, &sceneObjects.at(i), k);
        }
    }

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
