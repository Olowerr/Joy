#include "EasyLevel.h"


EasyLevel::EasyLevel(UIRenderer& uiRender, ObjectRender& objRender, DecalShadow& decalShadow, TempMeshStorage& meshStorage)
    :Scene(uiRender, objRender, decalShadow, meshStorage)
    , joy(meshStorage.GetMesh(0))
    , pickUpUI("../Resources/Images/BoltForJoy.png", 100.f, (float)Backend::GetWindowHeight() - 175.f, 0.7f, 0.7f)
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

    sceneObjects.reserve(20);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(2), true);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(2), true);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(2), true);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(1), true);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(1), true);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(1), true);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(1), true);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(1), true);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(3), true);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(7), true);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(7), true);
    sceneObjects.emplace_back(meshStorage.GetObjMesh(3), true, DirectX::XMFLOAT3(0.f, 2.f, 50.f));
    sceneObjects.emplace_back(meshStorage.GetObjMesh(3), true, DirectX::XMFLOAT3(0.f, 2.f, 55.f));

    ground = &sceneObjects[0];
    ground1 = &sceneObjects[1];
    ground2 = &sceneObjects[2];
    obstacle = &sceneObjects[3];
    obstacle1 = &sceneObjects[4];
    obstacle2 = &sceneObjects[5];
    obstacle3 = &sceneObjects[6];
    obstacle4 = &sceneObjects[7];
    portal = &sceneObjects[8];
    tree1 = &sceneObjects[9];
    tree2 = &sceneObjects[10];

    joy.SetPosition(0.0f, 5.0f, 0.0f);
    ground->SetPosition(0.0f, -2.0f, 0.0f);
    ground1->SetPosition(0.0f, -0.2f, 27.3f);
    ground2->SetPosition(5.1f, 2.4f, 64.2f);
    ground2->SetScale(2.8f);
    obstacle->SetPosition(-2.4f, 0.6f, 16.3f);
    obstacle->SetRotation(0.0f, 0.5f, 0.4f);
    obstacle->SetScale(0.4f);
    obstacle1->SetPosition(1.8f, -0.2f, 11.1f);
    obstacle1->SetScale(2.5f);
    obstacle2->SetPosition(1.5f, 3.0f, 15.5f);
    obstacle2->SetRotation(0.7f, 0.6f, 0.0f);
    obstacle2->SetScale(1.3f);
    obstacle3->SetPosition(2.8f, 2.5f, 31.4f);
    obstacle3->SetScale(4.4f);
    obstacle4->SetPosition(3.1f, 1.2f, 28.3f);
    obstacle4->SetScale(1.8f);
    portal->SetPosition(6.1f, 4.3f, 78.099f);
    portal->Scale(2.0f);
    tree1->SetPosition(17.2f, 4.8f, 68.2f);
    tree2->SetPosition(-7.1f, 4.8f, 58.7f);

    objRender.SetActiveCamera(activeCamera);
    decalShadow.SetActiveCamera(activeCamera);

    divider.CreateSections(2, 100.0f, 50.f, 50.);
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
    meshStorage.UnloadObjMeshes();
    Object::EmptyObjectLists();

    joy.Shutdown();

    for (Object& object : sceneObjects)
        object.Shutdown();

    freeCamera.Shutdown();
    joyCamera.Shutdown();

    divider.Shutdown();
    uiRender.Clear();
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

    if (coll1.getCollidedY() || coll2.getCollidedY() || coll3.getCollidedY() || coll4.getCollidedY() || coll5.getCollidedY() || 
        coll6.getCollidedY() || coll7.getCollidedY() || coll8.getCollidedY() || coll9.getCollidedY() || coll9.getCollidedY())
        joy.SetCanJump(true);
    else
        joy.SetCanJump(false);

    coll1.collided(&joy, ground);
    coll2.collided(&joy, ground1);
    coll3.collided(&joy, ground2);
    coll4.collided(&joy, obstacle);
    coll5.collided(&joy, obstacle1);
    coll6.collided(&joy, obstacle2);
    coll7.collided(&joy, obstacle3);
    coll8.collided(&joy, obstacle4);
    coll9.collided(&joy, tree1);
    coll10.collided(&joy, tree2);


    

    if (joy.GetBoundingBox().Intersects(portal->GetBoundingBox()))
    {
        
        m_highscore.DoAllTheHighscoreStuff(time); // send in the final score here


        return SceneState::Highscore;
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