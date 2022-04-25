#include "Game.h"
#include<iostream>
Game::Game(HINSTANCE hInstance, int cmdShow)
	:system(Backend::Create(hInstance, cmdShow, Win_Width, Win_Height))
	, window(Backend::GetWindow()) 
	, menu(uiRender, objRender, meshStorage)
	, easy(uiRender, objRender, meshStorage)
{


}

void Game::Shutdown()
{
	uiRender.Shutdown();
	objRender.Shutdown();

	Backend::Destroy();
}

void Game::Run()
{
	testScene test(uiRender, objRender, meshStorage);

	SceneState activeState = SceneState::Unchanged;
	Scene* activeScene = &menu;
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
		activeScene->Render();

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
		// --
	}

	activeScene->Shutdown();

}
