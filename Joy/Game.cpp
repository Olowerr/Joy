#include "Game.h"
#include<iostream>
Game::Game(HINSTANCE hInstance, int cmdShow)
	:system(Backend::Create(hInstance, cmdShow, Win_Width, Win_Height))
	, window(Backend::GetWindow()) 
{
	SetupImGui(window.GetHWND(), Backend::GetDevice(), Backend::GetDeviceContext());
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr))
		return;

	DirectX::AUDIO_ENGINE_FLAGS eflags = DirectX::AudioEngine_Default;
#ifdef _DEBUG
	eflags |= DirectX::AudioEngine_Debug;
#endif
	audEngine = std::make_unique<DirectX::AudioEngine>(eflags);

	soundEffect1 = std::make_unique<DirectX::SoundEffect>(audEngine.get(), L"../Resources/Sound/MenuLevelSound.wav");
	effect1 = soundEffect1->CreateInstance();
	effect1->SetVolume(0.1f);

	soundEffect2 = std::make_unique<DirectX::SoundEffect>(audEngine.get(), L"../Resources/Sound/EasyLevelSound.wav");
	effect2 = soundEffect2->CreateInstance();
	effect2->SetVolume(0.1f);

	soundEffect3 = std::make_unique<DirectX::SoundEffect>(audEngine.get(), L"../Resources/Sound/HighscoreLevelSound.wav");
	effect3 = soundEffect3->CreateInstance();
	effect3->SetVolume(0.1f);
}

void Game::Shutdown()
{
	smolpp.Shutdown();
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
	Scene* activeScene = new testScene(uiRender, objRender, decalShadow, meshStorage);
	//Scene* activeScene = new MainMenu(uiRender, objRender, decalShadow, meshStorage);
	effect1->Play();
	Backend::ResetDeltaTime();

	while (window.IsOpen())
	{
		switch (activeState)
		{
		default:
			break;
		case SceneState::MainMenu:
			effect3->Stop();
			activeScene->Shutdown();
			delete activeScene;
			activeScene = new MainMenu(uiRender, objRender, decalShadow, meshStorage);
			effect1->Play();
			Backend::ResetDeltaTime();
			break;

		case SceneState::Easy:
			effect1->Stop();
			activeScene->Shutdown();
			delete activeScene;
			activeScene = new EasyLevel(uiRender, objRender, decalShadow, meshStorage);
			effect2->Play();
			Backend::ResetDeltaTime();
			break;

		case SceneState::Highscore:
			effect1->Stop();
			effect2->Stop();
			activeScene->Shutdown();
			delete activeScene;
			activeScene = new HighscoreLevel(uiRender, objRender, decalShadow, meshStorage);
			effect3->Play();
			Backend::ResetDeltaTime();
			break;
		}
		
		Backend::Process();

		Backend::Clear();

		activeState = activeScene->Update();
		StartImGuiFrame();
		activeScene->Render();

		EndImGuiFrame();

		smolpp.ApplyGlow();
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
