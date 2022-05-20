#pragma once
#include <string>
#include <Keyboard.h>
#include "Text.h"
#include "UIRender.h"
#include "Scene.h"

class PausMenu
{
public:
	PausMenu(UIRenderer& uiRender);
	~PausMenu();
	void Shutdown();
	void AddRend();
	void Paus(SceneState currentState);
	SceneState& GetSceneState();
	bool isPaused;
	void SetActive(bool active);

private:
	Keyboard& m_key;
	UIRenderer* ui;
	int m_selection;
	Text m_pausText;
	Text m_arrow;
	bool doInput;
	SceneState state;

};

