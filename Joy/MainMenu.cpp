#include "MainMenu.h"

MainMenu::MainMenu(UIRenderer& uiRender, ObjectRender& objRender, DecalShadow& decalShadow, TempMeshStorage& meshStorage)
	:Scene(uiRender, objRender, decalShadow, meshStorage)
	, startButton("../Resources/Images/cat.png", (float)Backend::GetWindowWidth() * 0.5f, (float)Backend::GetWindowHeight() * 0.5f, 1.f, 1.f)
{

	uiRender.Clear();
	uiRender.Add(&startButton);

	Backend::GetDeviceContext()->RSSetViewports(1, &Backend::GetDefaultViewport());
}

void MainMenu::Shutdown()
{
	startButton.Shutdown();
}

SceneState MainMenu::Update()
{
	if (startButton.Clicked())
	{
		std::cout << "Clicked\n";
		return SceneState::Easy;
	}



	return SceneState::Unchanged;
}

void MainMenu::Render()
{
	uiRender.Draw();
}
