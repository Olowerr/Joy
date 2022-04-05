#include "MainMenu.h"

MainMenu::MainMenu(UIRenderer& uiRender, ObjectRender& objRender, MeshStorage& meshStorage)
	:Scene(uiRender, objRender, meshStorage)
	, startButton("../Resources/Images/cat.png")
{
	/*
	
	CHECK RENDERDOC
	CHECK RENDERDOC
	CHECK RENDERDOC
	CHECK RENDERDOC
	CHECK RENDERDOC
	CHECK RENDERDOC
	CHECK RENDERDOC

	*/
}

void MainMenu::Load()
{
	uiRender.Clear();
	uiRender.Add(&startButton);

	Backend::GetDeviceContext()->RSSetViewports(1, &Backend::GetStdViewport());
}

void MainMenu::Shutdown()
{
	startButton.Shutdown();
}

SceneState MainMenu::Update()
{
	if (startButton.Clicked())
		std::cout << "Clicked\n";

	return SceneState::Unchanged;
}

void MainMenu::Render()
{
	uiRender.Draw();
}
