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
    sceneObjects.emplace_back(meshStorage.GetMesh(3), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(3), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(3), true);

    ground = &sceneObjects[0];
    obstacle = &sceneObjects[1];
    obstacle1 = &sceneObjects[2];
    obstacle2 = &sceneObjects[3];

    joy.SetPosition(0.0f, 5.0f, 10.0f);
    ground->SetPosition(0.0f, 0.0f, 10.0f);
    ground->SetScale(2.0f);
    obstacle->SetPosition(-6.1f, 1.5f, 20.0f);
    obstacle1->SetPosition(0.7f, 1.5f, 20.0f);
    obstacle2->SetPosition(7.5f, 1.5f, 20.0f);

    hLight.GenerateLightMaps(divider);
    hLight.ShutdownTools();

    objRender.SetActiveCamera(activeCamera);
    decalShadow.SetActiveCamera(activeCamera);

    divider.CreateSections(1, 50.f, 50.f, 50.);
    objRender.SetMapDivier(&divider);
    decalShadow.SetMapDivider(&divider);
}

void MainMenu::Shutdown()
{
    hLight.Shutdown();

    //decalShadow.Shutdown();
    //objRender.Shutdown();
    uiRender.Shutdown();
    //objRender.Clear();
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
    joy.SetCollidedY(coll.getCollidedY());
    if (coll.getCollidedY())
        joy.SetSpeedZero();

    if (coll.HitObject(&joy, obstacle))
        joy.SetSpeedZero();
    if (coll.HitObject(&joy, obstacle1))
        joy.SetSpeedZero();
    if (coll.HitObject(&joy, obstacle2))
        joy.SetSpeedZero();

    if (coll.HitObject(&joy, ground))
    {
        joy.SetSpeedZero();
        joy.SetCanJump(coll.GetStopFall());
    }

    joy.SetCanJump(coll.GetStopFall());

    coll.collided(&joy, ground);
    if (joy.GetBoundingBox().Intersects(obstacle1->GetBoundingBox()))
    {
        return SceneState::Easy;
    }
    coll.collided(&joy, obstacle);
    coll.collided(&joy, obstacle2);

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
