#include "HighscoreLevel.h"

HighscoreLevel::HighscoreLevel(UIRenderer& uiRender, ObjectRender& objRender, DecalShadow& decalShadow, TempMeshStorage& meshStorage)
    :Scene(uiRender, objRender, decalShadow, meshStorage)
    , joy(meshStorage.GetMesh(0))
    , joyCamera(joy)
    , divider(joy)
    , activeCamera(&joyCamera)
{
    meshStorage.LoadAll();

    joy.CheckBB();

    sceneObjects.reserve(10);
    sceneObjects.emplace_back(meshStorage.GetMesh(2), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(3), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(5), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(5), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(4), true);

    ground1 = &sceneObjects[0];
    portal = &sceneObjects[1];
    wall1 = &sceneObjects[2];
    wall2 = &sceneObjects[3];
    wall3 = &sceneObjects[4];

    joy.SetPosition(0.0f, 3.0f, 0.0f);
    ground1->SetPosition(0.0f, 0.0f, 0.0f);
    ground1->SetScale(2.0f);
    portal->SetPosition(-7.4f, 2.0f, 9.5f);
    portal->SetRotation(0.0f, 0.0f, 0.0f);
    portal->SetScale(2.0f);
    wall1->SetPosition(9.9f, 1.9f, 0.0f);
    wall1->SetScale(2.0f);
    wall2->SetPosition(-9.9f, 1.9f, 0.0f);
    wall2->SetScale(2.0f);
    wall3->SetPosition(0.0f, 1.9f, 9.8f);
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

void HighscoreLevel::Shutdown()
{
    hLight.Shutdown();

    //uiRender.Shutdown();
    objRender.Clear();
    meshStorage.UnLoadAll();
   // Object::EmptyObjectLists();

    joy.Shutdown();

    for (Object& object : sceneObjects)
        object.Shutdown();

    freeCamera.Shutdown();
    joyCamera.Shutdown();

    divider.Shutdown();
}

SceneState HighscoreLevel::Update()
{
    joy.Jump();
    joy.Move();
    joy.Respawn();
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

    if (coll1.getCollidedY() || coll2.getCollidedY() || coll3.getCollidedY() || coll4.getCollidedY())
        joy.SetCanJump(true);
    else
        joy.SetCanJump(false);

    coll1.collided(&joy, ground1);
    coll2.collided(&joy, wall1);
    coll3.collided(&joy, wall2);
    coll4.collided(&joy, wall3);

    if (joy.GetBoundingBox().Intersects(portal->GetBoundingBox()))
    {
        return SceneState::MainMenu;
    }

    return SceneState::Unchanged;
}

void HighscoreLevel::Render()
{
    objRender.DrawAll();
    decalShadow.DrawAll(joy.GetPosition());
    objRender.DrawCharacter(joy);
}
