#include "Game.h"
#include<iostream>
Game::Game(HINSTANCE hInstance, int cmdShow)
	:system(Backend::Create(hInstance, cmdShow, Win_Width, Win_Height))
	, window(Backend::GetWindow()) 
{
	SetupImGui(window.GetHWND(), Backend::GetDevice(), Backend::GetDeviceContext());
}

void Game::Shutdown()
{
	uiRender.Shutdown();
	objRender.Shutdown();
	decalShadow.Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	Backend::Destroy();
}

void Game::Run()
{
	SceneState activeState = SceneState::Unchanged;
	//Scene* activeScene = new EasyLevel(uiRender, objRender, decalShadow, meshStorage);
	//Scene* activeScene = new testScene(uiRender, objRender, decalShadow, meshStorage);
	Scene* activeScene = new MainMenu(uiRender, objRender, decalShadow, meshStorage);

	while (window.IsOpen())
	{
		switch (activeState)
		{
		default:
			break;
		case SceneState::MainMenu:
			activeScene->Shutdown();
			delete activeScene;
			activeScene = new MainMenu(uiRender, objRender, decalShadow, meshStorage);
			break;

		case SceneState::Easy:
			activeScene->Shutdown();
			delete activeScene;
			activeScene = new EasyLevel(uiRender, objRender, decalShadow, meshStorage);
			break;

		case SceneState::Highscore:
			activeScene->Shutdown();
			delete activeScene;
			activeScene = new HighscoreLevel(uiRender, objRender, decalShadow, meshStorage);
			break;
		}
		
		Backend::Process();

		Backend::Clear();

		activeState = activeScene->Update();
		StartImGuiFrame();
		activeScene->Render();

		EndImGuiFrame();

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

	activeScene->Shutdown();
	delete activeScene;
}
