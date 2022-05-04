#include "EasyLevel.h"

EasyLevel::EasyLevel(UIRenderer& uiRender, ObjectRender& objRender, DecalShadow& decalShadow, TempMeshStorage& meshStorage)
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
    sceneObjects.emplace_back(meshStorage.GetMesh(2), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(2), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(1), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(1), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(1), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(1), true);
    sceneObjects.emplace_back(meshStorage.GetMesh(1), true);

    ground = &sceneObjects[0];
    ground1 = &sceneObjects[1];
    ground2 = &sceneObjects[2];
    obstacle = &sceneObjects[3];
    obstacle1 = &sceneObjects[4];
    obstacle2 = &sceneObjects[5];
    obstacle3 = &sceneObjects[6];
    obstacle4 = &sceneObjects[7];

    joy.SetPosition(0.0f, 3.0f, 10.0f);
    ground->SetPosition(0.0f, -2.0f, 10.0f);
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


    objRender.SetActiveCamera(activeCamera);
    decalShadow.SetActiveCamera(activeCamera);

    divider.CreateSections(1, 50.f, 50.f, 50.);
    objRender.SetMapDivier(&divider);
    decalShadow.SetMapDivider(&divider);

    hLight.InitiateTools(divider);
    hLight.GenerateLightMaps(divider);
    hLight.ShutdownTools();
}

void EasyLevel::Shutdown()
{
    hLight.Shutdown();

    decalShadow.Shutdown();
    objRender.Clear();
    meshStorage.UnLoadAll();

    joy.Shutdown();

    for (Object& object : sceneObjects)
        object.Shutdown();

    freeCamera.Shutdown();
    joyCamera.Shutdown();

    divider.Shutdown();
}

SceneState EasyLevel::Update()
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
    if (coll.HitObject(&joy, obstacle3))
        joy.SetSpeedZero();
    if (coll.HitObject(&joy, obstacle4))
        joy.SetSpeedZero();

    if (coll.HitObject(&joy, ground))
    {
        joy.SetSpeedZero();
        joy.SetCanJump(coll.GetStopFall());
    }
    if (coll.HitObject(&joy, ground1))
    {
        joy.SetSpeedZero();
        joy.SetCanJump(coll.GetStopFall());
    }
    if (coll.HitObject(&joy, ground2))
    {
        joy.SetSpeedZero();
        joy.SetCanJump(coll.GetStopFall());
    }

    joy.SetCanJump(coll.GetStopFall());

    coll.collided(&joy, ground);
    coll.collided(&joy, ground1);
    coll.collided(&joy, ground2);
    coll.collided(&joy, obstacle);
    coll.collided(&joy, obstacle1);
    coll.collided(&joy, obstacle2);
    coll.collided(&joy, obstacle3);
    coll.collided(&joy, obstacle4);

    //Joy functions
    joy.Jump();
    joy.Move();
    joy.Respawn();

    return SceneState::Unchanged;
}

void EasyLevel::Render()
{
	objRender.DrawAll();
    decalShadow.DrawAll(joy.GetPosition());
    objRender.DrawCharacter(joy);
}