#include "EasyLevel.h"


EasyLevel::EasyLevel(UIRenderer& uiRender, ObjectRender& objRender, DecalShadow& decalShadow, TempMeshStorage& meshStorage)
	:Scene(uiRender, objRender, decalShadow, meshStorage)
    , joy(&meshStorage.joy[0])
    , barUI("../Resources/Images/progressBar.png", 10.f, (float)Backend::GetWindowHeight() - 220, 1.f, 1.f)
    , arrow("../Resources/Images/arrow.png")
    , loadingScreen("../Resources/Images/LoadingScreen.png", 0.0f, 0.0f, 1.f, 1.f)
    , joyCamera(joy)
    , divider(joy)
    , activeCamera(&joyCamera)
    , m_highscore(uiRender)
 {
    SoundSystem::getInstance().StopSounds();
    meshStorage.LoadAllObj();
    uiRender.Add(&barUI);
    uiRender.Add(&arrow);
    uiRender.Add(&thomas);
    thomas.SetPosition(10.f, 10.f);
    thomas.SetColour(DirectX::Colors::BlueViolet);
    
    thomas.SetText("THOMAS");
    thomas.SetScale(1.5f, 1.5f);

    joy.CheckBB();

    typedef DirectX::XMFLOAT3 F3;

    meshStorage.LoadEasyObjects();

    sceneObjects.reserve(200);

    PlaceObjects();
    meshStorage.UnloadDataBase();

    collisions.reserve(sceneObjects.size());
    for (size_t i = 0; i < (int)sceneObjects.size(); i++)
        collisions.emplace_back();

    joy.SetPosition(0.0f, 5.0f, 0.0f);

    objRender.SetActiveCamera(activeCamera);
    decalShadow.SetActiveCamera(activeCamera);

    divider.CreateSections(2, 500.0f, 50.f, 50.f);
    objRender.SetMapDivier(&divider);
    decalShadow.SetMapDivider(&divider);

    hLight.InitiateTools(divider);

    InstancedObject::CreateInstancedObjects(meshStorage, divider, hLight);
    hLight.GenerateLightMaps(divider);

    hLight.ShutdownTools();

    sky.init();
    SoundSystem::getInstance().GetEffect(1)->Play(true);
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
    barUI.Shutdown();
    arrow.Shutdown();
    thomas.Shutdown();
    m_highscore.Shutdown();
}

SceneState EasyLevel::Update()
{
    
    //ProgressBar

    float distanceTrav = 30 + joy.GetPosition().z * 1.2;
    arrow.SetPosition(18.f, (float)Backend::GetWindowHeight() - distanceTrav);
    Backend::Clear();

    time += Backend::GetDeltaTime();

    auto asd = std::to_string(time);
    asd.erase(asd.find_first_of('.') + 3, std::string::npos);
    
    thomas.SetText(asd);

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
#endif // _DEBUG

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
        m_highscore.InputNameAndSetHighscore(time); // send in the final score here
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
#ifdef _DEBUG
    ImGuiModifyTransform(Object::GetLevelObjects(), activeCamera);
#endif // DEBUG
}

void EasyLevel::PlaceObjects()
{
    typedef DirectX::XMFLOAT3 F3;
    
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q10"), true, F3(10.000f, 6.500f, 140.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q10"), true, F3(-10.000f, 6.500f, 140.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q26"), true, F3(2.000f, -3.601f, 217.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q26"), true, F3(0.000f, -7.000f, 217.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q26"), true, F3(3.961f, -7.000f, 216.740f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q30"), true, F3(-13.521f, -7.129f, 237.670f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q30"), true, F3(-13.696f, -7.129f, 233.539f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q30"), true, F3(-11.799f, -7.129f, 235.497f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q30"), true, F3(-10.657f, -7.129f, 237.878f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q37"), true, F3(-9.476f, 2.157f, 352.319f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q37"), true, F3(9.432f, 2.157f, 352.319f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q37"), true, F3(0.000f, 2.157f, 382.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q37"), true, F3(0.000f, 2.157f, 424.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("RasmuOtherBox2"), true, F3(0.332f, 2.353f, 18.406f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("RasmuOtherBox2"), true, F3(0.332f, 2.353f, 14.554f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q47"), true, F3(14.466f, 12.000f, 44.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q47"), true, F3(14.466f, 12.000f, 52.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q47"), true, F3(14.466f, 12.000f, 60.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q47"), true, F3(14.466f, 12.000f, 68.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q47"), true, F3(14.466f, 12.000f, 90.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q47"), true, F3(14.466f, 12.000f, 98.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q47"), true, F3(14.466f, 12.000f, 106.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q6"), true, F3(-0.000f, 2.140f, 403.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q6"), true, F3(-0.000f, 2.140f, 444.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q6"), true, F3(10.469f, 12.000f, 86.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q6"), true, F3(5.000f, 12.000f, 94.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q6"), true, F3(10.469f, 12.000f, 102.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q6"), true, F3(5.000f, 12.000f, 110.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q4"), true, F3(0.000f, -2.000f, 258.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q4"), true, F3(0.000f, -4.000f, 253.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q4"), true, F3(0.000f, -6.000f, 248.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q4"), true, F3(0.000f, -8.000f, 243.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("OrangeCone1"), true, F3(0.409f, -7.909f, 196.248f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("OrangeCone1"), true, F3(2.927f, -7.909f, 196.610f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("OrangeCone1"), true, F3(6.295f, -7.909f, 196.016f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("OrangeCone1"), true, F3(9.141f, -7.909f, 196.422f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("OrangeCone1"), true, F3(11.651f, -7.909f, 196.422f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("OrangeCone1"), true, F3(-2.101f, -7.909f, 197.627f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("OrangeCone1"), true, F3(-5.122f, -7.909f, 196.313f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("OrangeCone1"), true, F3(-8.140f, -7.909f, 195.864f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("OrangeCone1"), true, F3(-11.262f, -7.909f, 197.033f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Cube11"), true, F3(4.177f, -8.348f, 204.152f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Cube11"), true, F3(9.317f, -4.604f, 230.515f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Flower1"), true, F3(-8.974f, 3.429f, 0.477f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Flower1"), true, F3(-3.023f, 2.100f, 297.938f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Flower1"), true, F3(5.263f, 2.101f, 305.555f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Flower1"), true, F3(-10.814f, 2.101f, 330.002f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Flower1"), true, F3(0.973f, 2.101f, 331.802f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Flower1"), true, F3(-13.818f, 11.722f, 89.272f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Flower1"), true, F3(-3.383f, 11.721f, 98.787f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Flower1"), true, F3(-4.358f, 11.720f, 53.207f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Shovel"), true, F3(-6.944f, -6.069f, 208.618f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Shovel"), true, F3(9.380f, -6.037f, 228.160f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Pipe_big"), true, F3(-13.544f, -0.490f, 227.700f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Pipe_big"), true, F3(13.544f, -0.490f, 227.700f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Pipe_small"), true, F3(-13.589f, -4.174f, 198.885f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Pipe_small"), true, F3(13.589f, -4.174f, 198.885f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Bench"), true, F3(8.198f, 2.674f, 3.420f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Bench"), true, F3(4.928f, 2.674f, 3.420f), F3(0.000f, DirectX::XM_PI, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Plant_box"), true, F3(-9.809f, 1.631f, 3.612f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Plant_box"), true, F3(-11.246f, 1.631f, 270.351f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Plant_box"), true, F3(11.270f, 1.631f, 270.351f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("tree_a2"), true, F3(-0.200f, 16.884f, 54.606f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("tree_a2"), true, F3(2.425f, 16.506f, 43.947f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("tree_a2"), true, F3(0.686f, 17.345f, 46.379f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("tree_a2"), true, F3(-1.084f, 16.198f, 48.923f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Bench2"), true, F3(8.702f, 2.674f, 280.869f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Bench2"), true, F3(-8.702f, 2.674f, 280.869f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("rock_a1"), true, F3(9.035f, 1.222f, 322.690f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("rock_a1"), true, F3(-9.326f, 1.222f, 318.337f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("rock_a1"), true, F3(-2.367f, 11.559f, 51.970f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("rock_a1"), true, F3(2.572f, 11.293f, 53.130f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("polySurface26"), true, F3(-11.830f, 15.129f, 69.940f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("polySurface26"), true, F3(-5.674f, 15.129f, 63.502f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("polySurface26"), true, F3(-3.038f, 15.129f, 72.782f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("polySurface26"), true, F3(-14.025f, 15.129f, 84.462f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("polySurface26"), true, F3(-4.175f, 15.129f, 88.115f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("polySurface20"), true, F3(-7.712f, 15.335f, 82.495f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("polySurface20"), true, F3(-6.820f, 15.335f, 95.889f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("polySurface20"), true, F3(-4.095f, 15.335f, 94.660f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("polySurface20"), true, F3(-2.527f, 15.335f, 98.080f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("polySurface7"), true, F3(-0.399f, 4.060f, 308.538f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("polySurface7"), true, F3(-1.450f, 4.060f, 337.824f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("polySurface7"), true, F3(-1.083f, 4.060f, 334.896f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("polySurface7"), true, F3(2.031f, 4.060f, 335.369f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("polySurface7"), true, F3(3.595f, 4.060f, 308.745f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("polySurface7"), true, F3(-12.346f, 11.571f, 59.512f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("rock_c3"), true, F3(8.480f, 2.213f, 292.909f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("rock_c3"), true, F3(9.000f, 1.183f, 293.772f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("rock_c3"), true, F3(7.487f, 1.183f, 293.245f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("rock_c3"), true, F3(8.895f, 1.183f, 291.968f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("rock_c3"), true, F3(-9.775f, 1.183f, 314.828f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("rock_c3"), true, F3(-9.775f, 1.183f, 330.780f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("rock_c3"), true, F3(-10.585f, 1.183f, 332.479f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("rock_c3"), true, F3(13.409f, 10.720f, 66.116f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("rock_c3"), true, F3(13.409f, 10.720f, 67.906f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("rock_c3"), true, F3(10.921f, 10.720f, 84.945f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("rock_c3"), true, F3(1.609f, 10.720f, 56.840f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Grass_garden_platform3"), true, F3(-13.386f, 2.599f, 268.920f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Grass_garden_platform3"), true, F3(12.372f, 2.097f, 268.355f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Grass_garden_platform3"), true, F3(10.049f, 2.097f, 271.155f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Grass_garden_platform3"), true, F3(-10.183f, 2.840f, 268.471f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Grass_garden_platform3"), true, F3(-10.736f, 2.097f, 337.961f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Grass_garden_platform3"), true, F3(-6.096f, 2.097f, 345.826f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Grass_garden_platform3"), true, F3(8.700f, 2.097f, 345.826f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Grass_garden_platform3"), true, F3(4.778f, 2.097f, 331.749f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Grass_garden_platform3"), true, F3(-4.078f, 2.097f, 323.338f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Grass_garden_platform3"), true, F3(6.927f, 2.097f, 296.983f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Grass_garden_platform3"), true, F3(-9.189f, 2.097f, 300.624f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Grass_garden_platform3"), true, F3(-7.234f, 2.097f, 292.013f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Grass_garden_platform3"), true, F3(-8.212f, 3.166f, 5.593f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Grass_garden_platform3"), true, F3(-7.372f, 3.166f, 0.708f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Engine_Base16"), true, F3(18.000f, -3.755f, 38.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Engine_Base16"), true, F3(25.000f, -1.784f, 48.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Engine_Base16"), true, F3(28.000f, 0.243f, 58.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Engine_Base16"), true, F3(24.000f, 2.230f, 68.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Engine_Base16"), true, F3(19.000f, 4.243f, 78.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q24"), true, F3(18.000f, -1.000f, 38.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q24"), true, F3(25.000f, 1.000f, 48.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q24"), true, F3(28.000f, 3.000f, 58.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q24"), true, F3(24.000f, 5.000f, 68.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q24"), true, F3(18.000f, 7.000f, 78.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q2"), true, F3(13.000f, 2.000f, 169.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q2"), true, F3(-13.000f, 2.000f, 169.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("tree_c"), true, F3(-11.993f, 12.156f, 270.279f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("tree_c"), true, F3(11.943f, 12.156f, 269.765f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Hedge1"), true, F3(13.453f, 4.182f, 343.828f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Hedge1"), true, F3(13.453f, 4.182f, 327.212f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Hedge1"), true, F3(13.453f, 4.182f, 310.813f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Hedge1"), true, F3(13.453f, 4.182f, 294.407f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Hedge1"), true, F3(-13.549f, 4.182f, 294.407f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Hedge1"), true, F3(-13.549f, 4.182f, 310.813f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Hedge1"), true, F3(-13.549f, 4.182f, 327.212f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Hedge1"), true, F3(-13.549f, 4.182f, 343.828f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Hedge14"), true, F3(-0.716f, 13.543f, 63.172f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Hedge14"), true, F3(-0.716f, 13.543f, 79.505f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Hedge14"), true, F3(-0.716f, 13.543f, 95.943f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Hedge14"), true, F3(-0.716f, 13.543f, 112.335f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Engine_Base2"), true, F3(-9.000f, -11.859f, -21.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Engine_Base2"), true, F3(9.000f, -11.859f, -21.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Engine_Base2"), true, F3(-9.000f, -1.859f, 43.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Engine_Base2"), true, F3(9.000f, -1.859f, 43.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Engine_Base2"), true, F3(9.000f, -1.859f, 117.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Engine_Base2"), true, F3(-9.000f, -1.859f, 117.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Engine_Base2"), true, F3(-9.000f, -20.859f, 203.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Engine_Base2"), true, F3(9.000f, -20.859f, 203.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Engine_Base2"), true, F3(-0.000f, -11.900f, 385.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Engine_Base2"), true, F3(-0.000f, -11.900f, 450.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Hedge10"), true, F3(8.539f, 4.048f, 284.787f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Hedge10"), true, F3(-9.039f, 4.048f, 284.787f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Hedge10"), true, F3(-0.208f, 4.048f, 350.194f), F3(0.000f, 0.000f, 0.000f), 1.000f);

    sceneObjects.emplace_back(meshStorage.GetMeshByName("Final_Sprint_Platform"), true, F3(0.000f, -1.750f, 417.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Start_Platform"), true, F3(0.000f, -1.750f, 0.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("TheRasmus1"), true, F3(-9.282f, 4.096f, 25.659f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("RasmusBox1"), true, F3(-9.145f, 1.506f, -3.288f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("RasmusBox"), true, F3(-11.146f, 2.272f, -2.537f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("polySurface23"), true, F3(8.466f, 13.013f, 49.239f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q32"), true, F3(-11.177f, 11.155f, 103.814f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q31"), true, F3(-10.329f, 12.761f, 77.747f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q27"), true, F3(9.130f, -6.809f, 230.222f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q13"), true, F3(-9.443f, -6.915f, 207.674f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q12"), true, F3(0.000f, 0.000f, 182.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q11"), true, F3(0.000f, 4.000f, 155.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q9"), true, F3(11.661f, -6.286f, 207.715f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Garden_Platform"), true, F3(0.000f, -1.750f, 315.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q3"), true, F3(0.000f, -10.750f, 218.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Q1"), true, F3(-0.000f, 8.262f, 80.000f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("Barrier1"), true, F3(5.506f, -7.743f, 200.639f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("tree_a3"), true, F3(0.170f, 19.794f, 51.257f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("tree_b"), true, F3(-9.435f, 7.727f, 3.786f), F3(0.000f, 0.000f, 0.000f), 1.000f);
    sceneObjects.emplace_back(meshStorage.GetMeshByName("rock_b"), true, F3(0.045f, 1.575f, 302.546f), F3(0.000f, 0.000f, 0.000f), 1.000f);
}
