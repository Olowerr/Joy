#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <Keyboard.h>
#include "Text.h"
#include "UIRender.h"



class HighscoreClass
{
public:

	HighscoreClass(UIRenderer& uiRender);
	void Shutdown();
	~HighscoreClass();

	void SetHighscore(float score, std::string name);
	bool LoadFromFile();
	void InputNameAndSetHighscore(float score);
	void SortScores();
	void RenderHighScoreText();
	void AddRend();
	void HighScoreSetPos();
private:

	Keyboard& m_key;
	UIRenderer* ui;
	std::ifstream m_myFile;
	std::fstream m_myWrite;
	float m_scores[3];
	std::string m_names[3];

	float finScore;
	float highscore;
	FILE* filePtr;
	bool doInput;
	Text m_text;
	Text m_nameText;
	Text m_screenRend;

};