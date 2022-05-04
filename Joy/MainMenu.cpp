#include "MainMenu.h"

MainMenu::MainMenu(UIRenderer& uiRender, ObjectRender& objRender, DecalShadow& decalShadow, TempMeshStorage& meshStorage)
	:Scene(uiRender, objRender, decalShadow, meshStorage)
	, startButton("../Resources/Images/cat.png", (float)Backend::GetWindowWidth() * 0.5f, (float)Backend::GetWindowHeight() * 0.5f, 1.f, 1.f)
    , joy(meshStorage.GetMesh(0))
    , joyCamera(joy)
    , divider(joy)
    , activeCamera(&joyCamera)
{
	uiRender.Clear();
	uiRender.Add(&startButton);

	Backend::GetDeviceContext()->RSSetViewports(1, &Backend::GetDefaultViewport());

    meshStorage.LoadAll();

    joy.CheckBB();

    sceneObjects.reserve(10);
    sceneObjects.emplace_back(meshStorage.GetMesh(2), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(2), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(3), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(4), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(4), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(4), true);

    ground = &sceneObjects[0];
    ground1 = &sceneObjects[1];
    portal = &sceneObjects[2];
    wall = &sceneObjects[3];
    wall1 = &sceneObjects[4];
    wall2 = &sceneObjects[5];

    joy.SetPosition(0.0f, 5.0f, 10.0f);
    ground->SetPosition(0.0f, 0.0f, 10.0f);
    ground->SetScale(2.0f);
    ground1->SetPosition(-8.0f, 0.0f, 29.8f);
    ground1->SetScale(2.0f);
    portal->SetPosition(0.7f, 1.5f, 20.0f);
    portal->SetScale(2.0f);
    wall->SetPosition(10.0f, 1.9f, 10.0f);
    wall->SetRotation(0.0f, 1.570f, 0.0f);
    wall->SetScale(2.0f);
    wall1->SetPosition(-10.0f, 1.9f, 10.0f);
    wall1->SetRotation(0.0f, -1.570f, 0.0f);
    wall1->SetScale(2.0f);
    wall2->SetPosition(4.9f, 1.9f, 20.0f);
    wall2->SetScale(2.0f);


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

    //uiRender.Shutdown();
    objRender.Clear();
    meshStorage.UnLoadAll();
    Object::EmptyObjectLists();

    joy.Shutdown();

    for (Object& object : sceneObjects)
        object.Shutdown();

    freeCamera.Shutdown();
    joyCamera.Shutdown();

    divider.Shutdown();
	startButton.Shutdown();
}

SceneState MainMenu::Update()
{
	//if (startButton.Clicked())
	//{
	//	std::cout << "Clicked\n";
	//	return SceneState::Easy;
	//}
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

    //Camera functions
    activeCamera->UpdateCam();
    activeCamera->SetView();

    //Collision


    coll.collided(&joy, ground);
    coll.collided(&joy, ground1);
    coll.collided(&joy, wall);
    coll.collided(&joy, wall1);
    coll.collided(&joy, wall2);
    if (joy.GetBoundingBox().Intersects(portal->GetBoundingBox()))
    {
        return SceneState::Easy;
    }

    //Joy functions
    joy.Jump();
    joy.Move();
    joy.Respawn();

    return SceneState::Unchanged;
}

void MainMenu::Render()
{
	//uiRender.Draw();
    objRender.DrawAll();
    decalShadow.DrawAll(joy.GetPosition());
    objRender.DrawCharacter(joy);
}
