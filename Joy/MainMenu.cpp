#include "MainMenu.h"

MainMenu::MainMenu(UIRenderer& uiRender, ObjectRender& objRender, DecalShadow& decalShadow, TempMeshStorage& meshStorage)
	:Scene(uiRender, objRender, decalShadow, meshStorage)
    , joy(meshStorage.GetObjMesh(0))
    , joyCamera(joy)
    , divider(joy)
    , activeCamera(&joyCamera)
{   
	Backend::GetDeviceContext()->RSSetViewports(1, &Backend::GetDefaultViewport());

    meshStorage.LoadAllObj();

    joy.CheckBB();

    sceneObjects.reserve(20);
    sceneObjects.emplace_back(meshStorage.GetMesh(10), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(11), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(13), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(12), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(14), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(15), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(16), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(17), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(18), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(19), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(20), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(21), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(22), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(23), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(24), true);
    for (int i = 0; i < (int)sceneObjects.size(); i++)
    {
        coll.emplace_back();
    }

    joy.SetPosition(0.0f, 5.0f, 0.0f);

    objRender.SetActiveCamera(activeCamera);
    decalShadow.SetActiveCamera(activeCamera);

    divider.CreateSections(1, 50.f, 50.f, 50.);
    objRender.SetMapDivier(&divider);
    decalShadow.SetMapDivider(&divider);

    hLight.InitiateTools(divider);
    hLight.GenerateLightMaps(divider);
    hLight.ShutdownTools();

}

void MainMenu::Shutdown()
{
    hLight.Shutdown();

    objRender.Clear();
    meshStorage.UnloadObjMeshes();
    Object::EmptyObjectLists();

    joy.Shutdown();

    for (Object& object : sceneObjects)
        object.Shutdown();

    freeCamera.Shutdown();
    joyCamera.Shutdown();

    divider.Shutdown();
	
    uiRender.Clear();
}

SceneState MainMenu::Update()
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
        if (i == 1)
        {
            continue;
        }
        for (int k = 0; k < sceneObjects.at(i).GetNumBboxes(); k++)
        {
            coll.at(i).collided(&joy, &sceneObjects.at(i), k);
        }
    }

    if (joy.GetBoundingBox(0).Intersects(sceneObjects.at(1).GetBoundingBox(0)))
    {
        return SceneState::Easy;
    }

    return SceneState::Unchanged;
}

void MainMenu::Render()
{
    objRender.DrawAll();
    decalShadow.DrawAll(joy.GetPosition());
    objRender.DrawCharacter(joy);
	uiRender.Draw();

}
