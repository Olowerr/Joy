#include "MainMenu.h"

MainMenu::MainMenu(UIRenderer& uiRender, ObjectRender& objRender, DecalShadow& decalShadow, TempMeshStorage& meshStorage)
    :Scene(uiRender, objRender, decalShadow, meshStorage)
    , joy(&meshStorage.joy[0])
    , loadingScreen("../Resources/Images/LoadingScreen.png", 0.0f, 0.0f, 1.f, 1.f)
    , joyCamera(joy)
    , divider(joy)
    , activeCamera(&joyCamera)
    , highscore(uiRender)
    , pausMenu(uiRender)
{
    Backend::GetDeviceContext()->RSSetViewports(1, &Backend::GetDefaultViewport());
    SoundSystem::getInstance().StopSounds();

    pausMenu.AddRend();
    joy.CheckBB();

    typedef DirectX::XMFLOAT3 F3;
    sceneObjects.reserve(110);

    meshStorage.LoadMenuObjects();

    sceneObjects.emplace_back(meshStorage.GetMesh(0), true, F3(1.6f, 1.9f, 2.3f));
    compi = new HelpComputer(meshStorage.GetMesh(1), joy, uiRender);

    for (size_t i = 2; i < meshStorage.GetMeshCount(); i++)
    {
        sceneObjects.emplace_back(meshStorage.GetMesh(i), true, F3()); 
    }
    sceneObjects.at(1).RemoveBBox(0);
    sceneObjects.at(1).SetPosition(0.6f, 1.0, 0.f);
    sceneObjects.at(1).SetRotation(0.f, 1.2f, 0.f);
    sceneObjects.at(1).SetScale(1.2f);

    sceneObjects.at(10).RemoveBBox(0);

    meshStorage.UnloadDataBase();

    sceneObjects.at(0).SetPosition(1.6f, 1.9f, 2.3f);

    std::cout << "0\n";

    collisions.reserve(110);
    for (size_t i = 0; i < (int)sceneObjects.size(); i++)
        collisions.emplace_back();

    joy.SetPosition(0.0f, 5.0f, 0.0f);

    //sceneObjects[2].SetPosition(11.2f, 0.0f, 8.1f);
    //sceneObjects[3].SetPosition(-10.8f, 0.0f, 8.1f);

    objRender.SetActiveCamera(activeCamera);
    decalShadow.SetActiveCamera(activeCamera);

    divider.CreateSections(1, 20.f, 15.f, 10.f, -10.f);
    objRender.SetMapDivier(&divider);
    decalShadow.SetMapDivider(&divider);
    std::cout << "1\n";

    hLight.InitiateTools(divider);
    std::cout << "2\n";

    InstancedObject::CreateInstancedObjects(meshStorage, divider, hLight);
    hLight.GenerateLightMaps(divider);

    hLight.ShutdownTools();

    sky.init();
    std::cout << "3\n";

    highscore.AddRend();
    highscore.HighScoreSetPos();
    activeCamera->UpdateCam();
    //activeCamera->SetView();
    std::cout << "4\n";

    SoundSystem::getInstance().GetEffect(0)->Play(true);
}

void MainMenu::Shutdown()
{
    compi->Shutdown();
    delete compi;

    sky.Shutdown();
    hLight.Shutdown();
    pausMenu.Shutdown();

    highscore.Shutdown();
    objRender.Clear();
    meshStorage.UnloadMeshes();
    Object::EmptyObjectLists();
    InstancedObject::DestroyInstancedObjects();

    joy.Shutdown();

    for (Object& object : sceneObjects)
        object.Shutdown();

    freeCamera.Shutdown();
    joyCamera.Shutdown();
    
    divider.Shutdown();
    uiRender.Clear();
    loadingScreen.Shutdown();
}

SceneState MainMenu::Update()
{
#ifdef _DEBUG


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

    if (activeCamera == &freeCamera)
    {
        activeCamera->UpdateCam();
        activeCamera->SetView();
        return SceneState::Unchanged;
    }

#endif // DEBUG

    if (compi->Check())
    {
        JoyPostProcess::SetActive(false);
        return SceneState::Unchanged;
    }
    else
    {
        uiRender.EnableAll();
        compi->SetActive(false);
        pausMenu.SetActive(false);
        JoyPostProcess::SetActive(true);
    }

    activeCamera->SetView();

    JoyPostProcess::CalcGlowAmount(joy.GetFuel());

    joy.Jump();
    joy.Move();
    joy.Respawn();


    //Collision

    for (size_t i = 0; i < (int)collisions.size(); i++)
    {
        if (collisions.at(i).getCollidedY())
        {
            joy.SetCanJump(true);
            break;
        }
        else
            joy.SetCanJump(false);
    }

    for (size_t i = 1; i < (int)collisions.size(); i++)
    {
        for (int k = 0; k < sceneObjects.at(i).GetNumBboxes(); k++)
        {
            collisions.at(i).collided(&joy, &sceneObjects.at(i), k);
        }
    }
    if (joy.GetBoundingBox(0).Intersects(sceneObjects.at(0).GetBoundingBox(0)))
    {
        uiRender.Clear();
        uiRender.Add(&loadingScreen);
        return SceneState::Easy;
    }

    pausMenu.Paus(SceneState::MainMenu);
    if (pausMenu.isPaused)
    {
        compi->SetActive(false);
        if (pausMenu.GetSceneState() == SceneState::MainMenu)
        {
            uiRender.Clear();
            uiRender.Add(&loadingScreen);
        }
        return pausMenu.GetSceneState();
    }
    else
    {
        return SceneState::Unchanged;
    }
}

void MainMenu::Render()
{
    highscore.RenderHighScoreText();


    if (!joy.GetBoundingBox(0).Intersects(sceneObjects.at(0).GetBoundingBox(0)))
    {
        objRender.DrawAll();
        decalShadow.DrawAll(joy.GetPosition());
        objRender.DrawCharacter(joy);
        sky.Draw(activeCamera);
        uiRender.Draw();
    }
    else
    {
        uiRender.Draw();
    }
#ifdef _DEBUG

    ImGuiModifyTransform(Object::GetLevelObjects(), activeCamera);

    ImGuiModifyTransform(Object::GetLevelObjects(), activeCamera);
    HObject::GetInstance().Draw(&joy, activeCamera, false, true, 0);
#endif // DEBUG
    HObject::GetInstance().Draw(&joy, activeCamera, false, true, 0);

}
