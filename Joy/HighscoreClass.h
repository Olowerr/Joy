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
	~HighscoreClass();

	void SetHighscore(float score, std::string name);
	void GetHighscore(float& scoreList, std::string& nameList);
	void RenderInputScreen(float score);
	void SortScores();

private:
	
	Keyboard& m_key;
	UIRenderer* ui;
	std::ifstream m_myFile;
	std::fstream m_myWrite;
	float m_scores[3];
	std::string m_names[3];

	float highscore;
	FILE* filePtr;
	bool doInput;
	Text m_text;
	Text m_nameText;
};

