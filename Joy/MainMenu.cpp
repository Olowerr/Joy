#include "MainMenu.h"

MainMenu::MainMenu(UIRenderer& uiRender, ObjectRender& objRender, DecalShadow& decalShadow, TempMeshStorage& meshStorage)
    :Scene(uiRender, objRender, decalShadow, meshStorage)
    , joy(meshStorage.GetObjMesh(0))
    , loadingScreen("../Resources/Images/LoadingScreen.png", 0.0f, 0.0f, 1.f, 1.f)
    , joyCamera(joy)
    , divider(joy)
    , activeCamera(&joyCamera)
    , highscore(uiRender)
{   
	Backend::GetDeviceContext()->RSSetViewports(1, &Backend::GetDefaultViewport());
    SoundSystem::getInstance().StopSounds();

    meshStorage.LoadAllObj();

    joy.CheckBB();

    typedef DirectX::XMFLOAT3 F3;
    sceneObjects.reserve(110);

    meshStorage.LoadMenuObjects();
    for (size_t i = 0; i < meshStorage.GetMeshCount(); i++)
    {
        sceneObjects.emplace_back(meshStorage.GetMesh(i), true);
        if (i == 1)
        {
            for (size_t i = 0; i < 2; i++)
            {
                sceneObjects.emplace_back(meshStorage.GetMesh(1), true);
            }
        }
    }
    meshStorage.UnloadDataBase();
    sceneObjects.at(0).SetPosition(1.6f, 1.9f, 2.3);

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

    hLight.InitiateTools(divider);

    InstancedObject::CreateInstancedObjects(meshStorage, divider, hLight);
    hLight.GenerateLightMaps(divider);

    hLight.ShutdownTools();

    sky.init();


    highscore.AddRend();
    highscore.HighScoreSetPos();
    activeCamera->UpdateCam();

    SoundSystem::getInstance().GetEffect(0)->Play(true);
}

void MainMenu::Shutdown()
{
    sky.Shutdown();
    hLight.Shutdown();

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


   activeCamera->SetView();

    if (activeCamera == &freeCamera)
        return SceneState::Unchanged;
    
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

    return SceneState::Unchanged;
#endif // DEBUG
}

void MainMenu::Render()
{
    highscore.RenderHighScoreText();


    if (!joy.GetBoundingBox(0).Intersects(sceneObjects.at(0).GetBoundingBox(0)))
    {
        objRender.DrawAll();
        decalShadow.DrawAll(joy.GetPosition());
        objRender.DrawCharacter(joy);
        uiRender.Draw();
        sky.Draw(activeCamera);
    }
    else
    {
        uiRender.Draw();
    }
#ifdef _DEBUG
    ImGuiModifyTransform(Object::GetLevelObjects(), activeCamera);
#endif // DEBUG
}
