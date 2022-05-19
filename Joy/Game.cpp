#include "Game.h"
#include<iostream>
Game::Game(HINSTANCE hInstance, int cmdShow)
	:system(Backend::Create(hInstance, cmdShow, Win_Width, Win_Height))
	, window(Backend::GetWindow())
	, loadingScreen("../Resources/Images/LoadingScreen.png", 0.0f, 0.0f, 1.f, 1.f)
{
#ifdef _DEBUG
	SetupImGui(window.GetHWND(), Backend::GetDevice(), Backend::GetDeviceContext());
#endif // DEBUG

	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr))
		return;

	uiRender.Add(&loadingScreen);
}

void Game::Shutdown()
{
	HObject::GetInstance().Shutdown();

	meshStorage.UnloadJoy();
	loadingScreen.Shutdown();
	smolpp.Shutdown();
	uiRender.Shutdown();
	objRender.Shutdown();
	decalShadow.Shutdown();
#ifdef _DEBUG
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif // DEBUG

	Backend::Destroy();
}

void Game::Run()
{
	uiRender.Draw();
	uiRender.Clear();
	Backend::GetDeviceContext()->CopyResource(*Backend::GetBackBuffer(), *Backend::GetMainBuffer());
	Backend::Display();

	SceneState activeState = SceneState::Unchanged;
	Scene* activeScene = new EasyLevel(uiRender, objRender, decalShadow, meshStorage);
	//Scene* activeScene = new testScene(uiRender, objRender, decalShadow, meshStorage);
	//Scene* activeScene = new MainMenu(uiRender, objRender, decalShadow, meshStorage);
	Backend::ResetDeltaTime();

	while (window.IsOpen())
	{
		switch (activeState)
		{
		default:
			break;
		case SceneState::MainMenu:
			SoundSystem::getInstance().GetEffect(1)->Stop();
			activeScene->Shutdown();
			delete activeScene;
			activeScene = new MainMenu(uiRender, objRender, decalShadow, meshStorage);
			Backend::ResetDeltaTime();
			break;

		case SceneState::Easy:
			SoundSystem::getInstance().GetEffect(0)->Stop();
			activeScene->Shutdown();
			delete activeScene;
			activeScene = new EasyLevel(uiRender, objRender, decalShadow, meshStorage);
			Backend::ResetDeltaTime();
			break;
		}

		Backend::Process();

		Backend::Clear();
#ifdef _DEBUG
		StartImGuiFrame();
#endif // DEBUG

		activeState = activeScene->Update();
		activeScene->Render();
		smolpp.ApplyGlow();

#ifdef _DEBUG
		EndImGuiFrame();
#endif // DEBUG

		Backend::Display();

		// temp --
		if (Backend::GetKeyboard().KeyReleased(DIK_DELETE))
			break;
		
		else if (Backend::GetKeyboard().KeyReleased(DIK_F))
			Backend::GetSwapChain()->SetFullscreenState(TRUE, nullptr);

		else if (Backend::GetKeyboard().KeyReleased(DIK_G))
			Backend::GetSwapChain()->SetFullscreenState(FALSE, nullptr);

		else if (Backend::GetKeyboard().KeyReleased(DIK_F1) && activeState != SceneState::MainMenu)
			activeState = SceneState::MainMenu;

		else if (Backend::GetKeyboard().KeyReleased(DIK_F2) && activeState != SceneState::Easy)
			activeState = SceneState::Easy;

		else if (Backend::GetKeyboard().KeyReleased(DIK_M))
			Backend::GetMouse().Lock(true);

		else if (Backend::GetKeyboard().KeyReleased(DIK_N))
			Backend::GetMouse().Lock(false);
		// --
	}
	Backend::GetSwapChain()->SetFullscreenState(FALSE, nullptr);

	activeScene->Shutdown();
	delete activeScene;
}
