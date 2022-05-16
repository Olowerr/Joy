#include "EasyLevel.h"


EasyLevel::EasyLevel(UIRenderer& uiRender, ObjectRender& objRender, DecalShadow& decalShadow, TempMeshStorage& meshStorage)
	:Scene(uiRender, objRender, decalShadow, meshStorage)
    , joy(meshStorage.GetObjMesh(0))
    , pickUpUI("../Resources/Images/BoltForJoy.png", 10.f, (float)Backend::GetWindowHeight() - 173.f, 1.f, 1.f)
    , loadingScreen("../Resources/Images/loadingScreen.png", 0.0f, 0.0f, 1.f, 1.f)
    , joyCamera(joy)
    , divider(joy)
    , activeCamera(&joyCamera)
    , m_highscore(uiRender)
{
    meshStorage.LoadAllObj();

    uiRender.Add(&pickUpUI);
    uiRender.Add(&thomas);
    thomas.SetPosition(10.f, 10.f);
    thomas.SetColour(DirectX::Colors::BlueViolet);
    thomas.SetText("THOMAS");
    thomas.SetScale(1.5f, 1.5f);

    joy.CheckBB();

    typedef DirectX::XMFLOAT3 F3;
    sceneObjects.reserve(110);

    meshStorage.LoadEasyObjects();
    for (size_t i = 0; i < meshStorage.GetMeshCount(); i++)
    {
        sceneObjects.emplace_back(meshStorage.GetMesh(i), true);
    }
    meshStorage.UnloadDataBase();

    collisions.reserve(110);
    for (size_t i = 0; i < (int)sceneObjects.size(); i++)
        collisions.emplace_back();

    joy.SetPosition(0.0f, 5.0f, 0.0f);

    objRender.SetActiveCamera(activeCamera);
    decalShadow.SetActiveCamera(activeCamera);

    divider.CreateSections(2, 100.0f, 50.f, 50.);
    objRender.SetMapDivier(&divider);
    decalShadow.SetMapDivider(&divider);

    hLight.InitiateTools(divider);

    InstancedObject::CreateInstancedObjects(meshStorage, divider, hLight);
    hLight.GenerateLightMaps(divider);

    hLight.ShutdownTools();

    sky.init();
}

void EasyLevel::Shutdown()
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
    pickUpUI.Shutdown();
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
        m_highscore.DoAllTheHighscoreStuff(time); // send in the final score here
        uiRender.Clear();
        uiRender.Add(&loadingScreen);

        return SceneState::MainMenu;
    }

    return SceneState::Unchanged;
}

void EasyLevel::Render()
{
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

    ImGuiModifyTransform(Object::GetLevelObjects(), activeCamera);

}