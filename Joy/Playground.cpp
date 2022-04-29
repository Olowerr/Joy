#include "playground.h"

testScene::testScene(UIRenderer& uiRender, ObjectRender& objRender, DecalShadow& decalShadow, TempMeshStorage& meshStorage)
    :Scene(uiRender, objRender, decalShadow, meshStorage), joy(nullptr), bg(nullptr), collTest(nullptr), ground(nullptr)

{
}

void testScene::Load()
{
    ID3D11DeviceContext* devContext = Backend::GetDeviceContext();

    meshStorage.LoadAll();
    joy = new Character(meshStorage.GetMesh(0));
    collTest = new Object(meshStorage.GetMesh(1));
    //test->SetPosition(0.0f, 0.2f, .0f);
    bg = new Object(meshStorage.GetMesh(1));
    ground = new Object(meshStorage.GetMesh(2));
    camera = new CharacterCamera(*joy);
    objRender.AddObject(joy);
    objRender.AddObject(ground);
    objRender.AddObject(bg);
    ground->SetPosition(0.0f, -2.0f, 0.0f);
    bg->SetPosition(-1.0f, 1.0f, -5.0f);
    collTest->SetPosition(-2.0f, 0.0f, 0.0f);
    //bg->SetScale(1.f / 23.f);

  //  bg->Scale(2);

    // Create decal buffers. Camera and character pos as dynamic constant buffers.
    decalShadow.CreateCharacterDecal(joy);
    decalShadow.CreateDecalDepthCam(joy);

    // Add Comment
    viewAndProj = camera->GetViewAndProj();
    Backend::CreateDynamicCBuffer(&camCb, &viewAndProj, 64);

    objRender.AddObject(collTest);
}

void testScene::Shutdown()
{
    objRender.Clear();
    meshStorage.UnLoadAll();
    joy->Shutdown();
    bg->Shutdown();
    ground->Shutdown();
    delete ground;
    delete camera;
    collTest->Shutdown();
    delete collTest;
    delete joy;
    delete bg;
}

SceneState testScene::Update()
{
    ID3D11DeviceContext* devContext = Backend::GetDeviceContext();
    viewAndProj = camera->GetViewAndProj();
    devContext->VSSetConstantBuffers(1, 1, &camCb);
    Backend::UpdateBuffer(camCb, &viewAndProj, 64);

    //ground->SetPosition(0, -2, 0);
    //Camera
    camera->UpdateCam();
    camera->SetView();
    //test->SetStopMovement(coll.GetDontStopMovement());
    joy->SetCanJump(false);
    joy->setCollidedY(coll.getCollidedY());
    if (coll.hitObject(joy, collTest))
        joy->setSpeedZero();
    if (coll.hitObject(joy, ground))
        joy->SetCanJump(coll.GetStopFall());
    if (coll.hitObject(joy, collTest))
        joy->SetCanJump(coll.GetStopFall());
    
    coll.collided(joy, collTest);
    //coll.collided(test, ground);
    //Test Character(cube)
    joy->Jump();
    joy->move();
    joy->respawn();

    // Update Character and Camera pos for the buffers.
    decalShadow.UpdateCharacterDecal(joy);
    decalShadow.UpdateDecalDepthCam(joy);

    //Collision
    

    return SceneState::Unchanged;
}

void testScene::Render()
{
    objRender.DrawAll();
}
