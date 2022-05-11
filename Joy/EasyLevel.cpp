#include "EasyLevel.h"

EasyLevel::EasyLevel(UIRenderer& uiRender, ObjectRender& objRender, DecalShadow& decalShadow, TempMeshStorage& meshStorage)
	:Scene(uiRender, objRender, decalShadow, meshStorage)
    , joy(meshStorage.GetMesh(0))
    , catButton("../Resources/Images/cat.png", 10.f, (float)Backend::GetWindowHeight() - 173.f, 1.f, 1.f)
    , joyCamera(joy)
    , divider(joy)
    , activeCamera(&joyCamera)
{
    meshStorage.LoadAll();

    uiRender.Add(&catButton);
    uiRender.Add(&thomas);
    thomas.SetPosition(10.f, 10.f);
    thomas.SetColour(DirectX::Colors::BlueViolet);
    thomas.SetText("THOMAS");
    thomas.SetScale(1.5f, 1.5f);

    joy.CheckBB();

    sceneObjects.reserve(20);
    sceneObjects.emplace_back(meshStorage.GetMesh(2), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(2), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(2), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(1), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(1), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(1), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(1), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(1), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(11), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(7), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(7), true);
    for (int i = 0; i < (int)sceneObjects.size(); i++)
    {
        coll.emplace_back();
    }

    joy.SetPosition(0.0f, 5.0f, 0.0f);
    sceneObjects[0].SetPosition(0.0f, -2.0f, 0.0f);
    sceneObjects[1].SetPosition(0.0f, -0.2f, 27.3f);
    sceneObjects[2].SetPosition(5.1f, 2.4f, 64.2f);
    sceneObjects[2].SetScale(2.8f);
    sceneObjects[3].SetPosition(-2.4f, 0.6f, 16.3f);
    sceneObjects[3].SetRotation(0.0f, 0.5f, 0.4f);
    sceneObjects[3].SetScale(0.4f);
    sceneObjects[4].SetPosition(1.8f, -0.2f, 11.1f);
    sceneObjects[4].SetScale(2.5f);
    sceneObjects[5].SetPosition(1.5f, 3.0f, 15.5f);
    sceneObjects[5].SetRotation(0.7f, 0.6f, 0.0f);
    sceneObjects[5].SetScale(1.3f);
    sceneObjects[6].SetPosition(2.8f, 2.5f, 31.4f);
    sceneObjects[6].SetScale(4.4f);
    sceneObjects[7].SetPosition(3.1f, 1.2f, 28.3f);
    sceneObjects[7].SetScale(1.8f);
    sceneObjects[8].SetPosition(-2.8f, -2.3f, 62.5f);
    sceneObjects[9].SetPosition(17.2f, 4.8f, 68.2f);
    sceneObjects[10].SetPosition(-7.1f, 4.8f, 58.7f);

    objRender.SetActiveCamera(activeCamera);
    decalShadow.SetActiveCamera(activeCamera);

    divider.CreateSections(1, 100.0f, 50.f, 50.);
    objRender.SetMapDivier(&divider);
    decalShadow.SetMapDivider(&divider);

    hLight.InitiateTools(divider);
    hLight.GenerateLightMaps(divider);
    hLight.ShutdownTools();
}

void EasyLevel::Shutdown()
{
    hLight.Shutdown();

    objRender.Clear();
    meshStorage.UnLoadAll();
    Object::EmptyObjectLists();

    joy.Shutdown();

    for (Object& object : sceneObjects)
        object.Shutdown();

    freeCamera.Shutdown();
    joyCamera.Shutdown();

    divider.Shutdown();
    uiRender.Clear();
    catButton.Shutdown();
    thomas.Shutdown();
}

SceneState EasyLevel::Update()
{
    time += Backend::GetDeltaTime();
    auto asd = std::to_string(time);
    asd.erase(asd.find_first_of('.') + 3, std::string::npos);
    thomas.SetText(asd);

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
        if (i == 8)
        {
            continue;
        }
        for (int k = 0; k < sceneObjects.at(i).GetNumBboxes(); k++)
        {
            coll.at(i).collided(&joy, &sceneObjects.at(i), k);
        }
    }

    if (joy.GetBoundingBox(0).Intersects(sceneObjects.at(8).GetBoundingBox(0)))
    {
        return SceneState::MainMenu;
    }

    return SceneState::Unchanged;
}

void EasyLevel::Render()
{
	objRender.DrawAll();
    decalShadow.DrawAll(joy.GetPosition());
    objRender.DrawCharacter(joy);
    uiRender.Draw();
}