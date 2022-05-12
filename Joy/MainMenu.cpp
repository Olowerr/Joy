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

    typedef DirectX::XMFLOAT3 F3;
    sceneObjects.reserve(110);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(11), true);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(12), true);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(13), true);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(14), true);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(15), true);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(16), true);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(17), true);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(18), true);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(19), true);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(20), true);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(21), true);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(22), true);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(23), true);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(24), true);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(25), true);
    for (int i = 0; i < (int)sceneObjects.size(); i++)
    {
        coll.emplace_back();
    }

    joy.SetPosition(0.0f, 5.0f, 0.0f);

    objRender.SetActiveCamera(activeCamera);
    decalShadow.SetActiveCamera(activeCamera);

    divider.CreateSections(1, 50.f, 15.f, 10.f);
    objRender.SetMapDivier(&divider);
    decalShadow.SetMapDivider(&divider);

    hLight.InitiateTools(divider);

    InstancedObject::CreateInstancedObjects(meshStorage, divider, hLight);
    hLight.GenerateLightMaps(divider);

    hLight.ShutdownTools();

    sky.init();
}

void MainMenu::Shutdown()
{
    sky.Shutdown();
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
    sky.Draw(activeCamera);
}
