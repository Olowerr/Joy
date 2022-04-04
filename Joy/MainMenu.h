#pragma once
#include "Scene.h"

class MainMenu : public Scene
{
public:
	MainMenu();

	// Inherited via Scene
	virtual void Load() override;
	virtual void Shutdown() override;
	virtual void Update() override;
	virtual void Render() override;

};