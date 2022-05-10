#include "MainMenu.h"

MainMenu::MainMenu(UIRenderer& uiRender, ObjectRender& objRender, DecalShadow& decalShadow, TempMeshStorage& meshStorage)
	:Scene(uiRender, objRender, decalShadow, meshStorage)
    , joy(meshStorage.GetMesh(0))
    , joyCamera(joy)
    , divider(joy)
    , activeCamera(&joyCamera)
{   
	Backend::GetDeviceContext()->RSSetViewports(1, &Backend::GetDefaultViewport());

    meshStorage.LoadAllObj();

    joy.CheckBB();

    sceneObjects.reserve(10);
    sceneObjects.emplace_back(meshStorage.GetMesh(2), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(3), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(3), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(5), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(5), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(4), true);

    ground1 = &sceneObjects[0];
    portal1 = &sceneObjects[1];
    portal2 = &sceneObjects[2];
    wall1 = &sceneObjects[3];
    wall2 = &sceneObjects[4];
    wall3 = &sceneObjects[5];

    joy.SetPosition(0.0f, 5.0f, 0.0f);
    ground1->SetPosition(0.0f, 0.0f, 0.0f);
    ground1->SetScale(2.0f);
    portal1->SetPosition(-3.1f, 1.5f, 10.0f);
    portal1->SetScale(2.0f);
    portal2->SetPosition(4.3f, 1.5f, 10.0f);
    portal2->SetScale(2.0f);
    wall1->SetPosition(10.0f, 1.9f, 0.0f);
    wall1->SetScale(2.0f);
    wall2->SetPosition(-10.0f, 1.9f, 0.0f);
    wall2->SetScale(2.0f);
    wall3->SetPosition(0.0f, 1.9f, 10.0f);
    wall3->SetScale(2.0f);

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
    meshStorage.UnloadMeshes();
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

    if (coll1.getCollidedY() || coll2.getCollidedY() || coll3.getCollidedY() || coll4.getCollidedY())
        joy.SetCanJump(true);
    else
        joy.SetCanJump(false);

    coll1.collided(&joy, ground1);
    coll2.collided(&joy, wall1);
    coll3.collided(&joy, wall2);
    coll4.collided(&joy, wall3);

    if (joy.GetBoundingBox().Intersects(portal1->GetBoundingBox()))
    {
        return SceneState::Easy;
    }

    if (joy.GetBoundingBox().Intersects(portal2->GetBoundingBox()))
    {
        return SceneState::Highscore;
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
