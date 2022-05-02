#include "Game.h"
#include<iostream>
Game::Game(HINSTANCE hInstance, int cmdShow)
	:system(Backend::Create(hInstance, cmdShow, Win_Width, Win_Height))
	, window(Backend::GetWindow()) 
	, menu(uiRender, objRender, meshStorage)
	, easy(uiRender, objRender, meshStorage)
{
	SetupImGui(window.GetHWND(), Backend::GetDevice(), Backend::GetDeviceContext());
}

void Game::Shutdown()
{
	uiRender.Shutdown();
	objRender.Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	Backend::Destroy();
}

void Game::Run()
{
	testScene test(uiRender, objRender, meshStorage);

	SceneState activeState = SceneState::Unchanged;
	Scene* activeScene = &test;
	activeScene->Load();

	while (window.IsOpen())
	{
		switch (activeState)
		{
		default:
			break;
		case SceneState::MainMenu:
			activeScene->Shutdown();
			activeScene = &menu;
			activeScene->Load();
			break;

		case SceneState::Easy:
			activeScene->Shutdown();
			activeScene = &easy;
			activeScene->Load();
			break;

		/*case SceneState::Medium:
			activeScene->Shutdown();
			activeScene = &medium;
			activeScene->Load();
			break;*/

		/*case SceneState::Hard:
			activeScene->Shutdown();
			activeScene = &hard;
			activeScene->Load();
			break;*/

		}
		
		Backend::Process();

		Backend::Clear();

		activeState = activeScene->Update();
		StartImGuiFrame();
		activeScene->Render();
		Backend::GetDeviceContext()->OMSetRenderTargets(1, Backend::GetBackBufferRTV(), nullptr);
		EndImGuiFrame();

		Backend::Display();

		// temp --
		if (Backend::GetKeyboard().KeyReleased(DIK_DELETE))
			break;

		else if (Backend::GetKeyboard().KeyReleased(DIK_F))
			Backend::GetSwapChain()->SetFullscreenState(TRUE, nullptr);

		else if (Backend::GetKeyboard().KeyReleased(DIK_G))
			Backend::GetSwapChain()->SetFullscreenState(FALSE, nullptr);

		else if (Backend::GetKeyboard().KeyReleased(DIK_1) && activeScene != &menu)
			activeState = SceneState::MainMenu;

		else if (Backend::GetKeyboard().KeyReleased(DIK_2) && activeScene != &easy)
			activeState = SceneState::Easy;

		else if (Backend::GetKeyboard().KeyReleased(DIK_M))
			Backend::GetMouse().Lock(true);
		
		else if (Backend::GetKeyboard().KeyReleased(DIK_N))
			Backend::GetMouse().Lock(false);
		// --
	}

	activeScene->Shutdown();

}
