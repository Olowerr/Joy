#include "HighscoreClass.h"

HighscoreClass::HighscoreClass(UIRenderer& uiRenderer)
	:m_scores(), m_names(), m_key(Backend::GetKeyboard()), doInput(true), m_screenRend()
{
	ui = &uiRenderer;

}

HighscoreClass::~HighscoreClass()
{
	Backend::Clear();
}

void HighscoreClass::SetHighscore(float score, std::string user)
{
	float finalScore = 1000 - score;

	if (finalScore > m_scores[0])
	{
		m_scores[0] = finalScore;
		m_names[0] = user;
	}
	else if (finalScore > m_scores[1])
	{
		m_scores[1] = finalScore;
		m_names[1] = user;
	}
	else if (finalScore > m_scores[2])
	{
		m_scores[2] = finalScore;
		m_names[2] = user;
	}

	SortScores();

	m_myWrite.open("../Resources/Highscore/highscore.txt", std::ios::out);

	m_myWrite << m_names[0] << std::endl;
	m_myWrite << m_scores[0] << std::endl;
	m_myWrite << m_names[1] << std::endl;
	m_myWrite << m_scores[1] << std::endl;
	m_myWrite << m_names[2] << std::endl;
	m_myWrite << m_scores[2] << std::endl;

	m_myWrite.close();

}

bool HighscoreClass::LoadFromFile()
{
	try
	{
		m_myFile.open("../Resources/Highscore/highscore.txt");

		if (m_myFile.is_open())
		{
			m_myFile >> m_names[0];
			m_myFile >> m_scores[0];
			m_myFile >> m_names[1];
			m_myFile >> m_scores[1];
			m_myFile >> m_names[2];
			m_myFile >> m_scores[2];

			m_myFile.close();
		}

		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}

}

void HighscoreClass::InputNameAndSetHighscore(float score)
{

	std::string hsText = "New High Score: " + std::to_string(score).erase(std::to_string(score).find_first_of('.') + 3, std::string::npos) + "\nEnter you name:";
	m_text.SetText(hsText);
	m_text.SetPosition((float)Backend::GetWindowWidth() / 2.f - 220.f, (float)Backend::GetWindowHeight() / 2.f - 200.f);
	m_nameText.SetPosition((float)Backend::GetWindowWidth() / 2.f - 100.f, (float)Backend::GetWindowHeight() / 2.f);
	ui->Add(&m_nameText);
	ui->Add(&m_text);

	std::string finalName;

	while (doInput)
	{

#pragma region //INPUT KEYS
		if (m_key.KeyReleased(DIK_A))
		{
			finalName.append("A");
		}
		if (m_key.KeyReleased(DIK_B))
		{
			finalName.append("B");
		}
		if (m_key.KeyReleased(DIK_C))
		{
			finalName.append("C");
		}
		if (m_key.KeyReleased(DIK_D))
		{
			finalName.append("D");
		}
		if (m_key.KeyReleased(DIK_E))
		{
			finalName.append("E");
		}
		if (m_key.KeyReleased(DIK_F))
		{
			finalName.append("F");
		}
		if (m_key.KeyReleased(DIK_G))
		{
			finalName.append("G");
		}
		if (m_key.KeyReleased(DIK_H))
		{
			finalName.append("H");
		}
		if (m_key.KeyReleased(DIK_I))
		{
			finalName.append("I");
		}
		if (m_key.KeyReleased(DIK_J))
		{
			finalName.append("J");
		}
		if (m_key.KeyReleased(DIK_K))
		{
			finalName.append("K");
		}
		if (m_key.KeyReleased(DIK_L))
		{
			finalName.append("L");
		}
		if (m_key.KeyReleased(DIK_M))
		{
			finalName.append("M");
		}
		if (m_key.KeyReleased(DIK_N))
		{
			finalName.append("N");
		}
		if (m_key.KeyReleased(DIK_O))
		{
			finalName.append("O");
		}
		if (m_key.KeyReleased(DIK_P))
		{
			finalName.append("P");
		}
		if (m_key.KeyReleased(DIK_Q))
		{
			finalName.append("Q");
		}
		if (m_key.KeyReleased(DIK_R))
		{
			finalName.append("R");
		}
		if (m_key.KeyReleased(DIK_C))
		{
			finalName.append("R");
		}
		if (m_key.KeyReleased(DIK_S))
		{
			finalName.append("S");
		}
		if (m_key.KeyReleased(DIK_T))
		{
			finalName.append("T");
		}
		if (m_key.KeyReleased(DIK_U))
		{
			finalName.append("U");
		}
		if (m_key.KeyReleased(DIK_V))
		{
			finalName.append("V");
		}
		if (m_key.KeyReleased(DIK_X))
		{
			finalName.append("X");
		}
		if (m_key.KeyReleased(DIK_Y))
		{
			finalName.append("Y");
		}
		if (m_key.KeyReleased(DIK_Z))
		{
			finalName.append("Z");
		}
		if (m_key.KeyReleased(DIK_4))
		{
			finalName.append("420");
		}
		if (m_key.KeyReleased(DIK_6))
		{
			finalName.append("X69");
		}
		if (m_key.KeyReleased(DIK_BACKSPACE))
		{
			if (finalName.size() > 0)
			{
				finalName.pop_back();
			}
		}
#pragma endregion

		m_nameText.SetText(finalName);

		Backend::Process();
		Backend::Clear();

		ui->Draw();

		Backend::GetDeviceContext()->CopyResource(*Backend::GetBackBuffer(), *Backend::GetMainBuffer());
		Backend::Display();


		if (m_key.KeyDown(DIK_RETURN))
		{
			LoadFromFile();
			if (finalName.size() > 10)
			{
				finalName.erase(+10, std::string::npos);
			}

			SetHighscore(score, finalName);

			m_nameText.Shutdown();
			m_text.Shutdown();
			doInput = false;
		}
	}
}


void HighscoreClass::SortScores()
{
	int min = 0;
	int max = 0;

	for (int i = 0; i < 2; i++)
	{
		min = i;

		for (int j = i + 1; j < 3; j++)
		{
			if (m_scores[j] > m_scores[min])
			{
				min = j;
			}

		}

		std::swap(m_scores[i], m_scores[min]);
		std::swap(m_names[i], m_names[min]);
	}

}

void HighscoreClass::RenderHighScoreText()
{
	m_myFile.open("../Resources/Highscore/highscore.txt");

	if (m_myFile.is_open())
	{
		m_myFile >> m_names[0];
		m_myFile >> m_scores[0];
		m_myFile >> m_names[1];
		m_myFile >> m_scores[1];
		m_myFile >> m_names[2];
		m_myFile >> m_scores[2];
	}

	m_myFile.close();

	std::string hsList = m_names[0] + ": " + std::to_string(m_scores[0]).erase(std::to_string(m_scores[0]).find_first_of('.') + 3, std::string::npos) + "\n\n" + m_names[1] + ": " + std::to_string(m_scores[1]).erase(std::to_string(m_scores[1]).find_first_of('.') + 3, std::string::npos) + "\n\n" + m_names[2] + ": " + std::to_string(m_scores[2]).erase(std::to_string(m_scores[2]).find_first_of('.') + 3, std::string::npos);

	m_screenRend.SetText(hsList);
}

void HighscoreClass::AddRend()
{
	ui->Add(&m_screenRend);
}

void HighscoreClass::HighScoreSetPos()
{
	m_screenRend.SetPosition(470.f, 320.f);
	m_screenRend.SetScale(0.5f, 0.5f);
	m_screenRend.SetColour({ 0.2f ,0.8f ,0.33f , 0.2f });
}
