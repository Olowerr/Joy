#include "Game.h"
#include<iostream>
Game::Game(HINSTANCE hInstance, int cmdShow)
	:system(Backend::Create(hInstance, cmdShow, Win_Width, Win_Height))
	, window(Backend::GetWindow()) 
	, menu(uiRender, objRender, meshStorage)
	, easy(uiRender, objRender, meshStorage)
{
	uiRender.Initiate();
	objRender.Initiate();

}

void Game::Shutdown()
{
	menu.Shutdown();
	easy.Shutdown();

	uiRender.Shutdown();
	objRender.Shutdown();
	meshStorage.Shutdown();

	Backend::Shutdown();
}

void Game::Run()
{
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

		// temp
		if (Backend::GetKeyboard().KeyDown(DIK_DELETE))
			break;

		Backend::Clear();

		activeState = activeScene->Update();
		activeScene->Render();

		Backend::Display();

	}

}
