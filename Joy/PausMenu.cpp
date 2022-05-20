#include "PausMenu.h"

PausMenu::PausMenu(UIRenderer& uiRender)
	:m_key(Backend::GetKeyboard()), m_selection(), doInput(true)
{
	ui = &uiRender;

	std::string pausText = ("CONTINUE \n\nMAIN MENU \n\nRESTART \n\nQUIT GAME");

	m_pausText.SetPosition(100.f, 100.f);
	m_pausText.SetText(pausText);
	m_arrow.SetText(">");
	m_arrow.SetPosition(80.f, 100.f);


}

PausMenu::~PausMenu()
{

}

void PausMenu::Shutdown()
{
	m_pausText.Shutdown();
	m_arrow.Shutdown();
}

void PausMenu::AddRend()
{
	ui->Add(&m_pausText);
	ui->Add(&m_arrow);
}

void PausMenu::Paus()
{

	if (m_key.KeyDown(DIK_P))
	{


		while (doInput)
		{

			if (m_key.KeyReleased(DIK_DOWNARROW)&& m_selection < 4 )
			{
				m_selection++;
			}


			if (m_key.KeyReleased(DIK_UPARROW) && m_selection > 0)
			{
				m_selection--;
			}


			if (m_selection == 0)
				m_arrow.SetPosition(80.f, 100.f);


			if (m_selection == 1)
				m_arrow.SetPosition(80.f, 192.f);


			if (m_selection == 2)
				m_arrow.SetPosition(80.f, 284.f);


			if (m_selection == 3)
				m_arrow.SetPosition(80.f, 376.f);


			Backend::Process();
			Backend::Clear();

			ui->Draw();

			Backend::GetDeviceContext()->CopyResource(*Backend::GetBackBuffer(), *Backend::GetMainBuffer());
			Backend::Display();

			if (m_key.KeyDown(DIK_RETURN))
			{
				doInput = false;
				m_arrow.Shutdown();
				m_pausText.Shutdown();
			}





		}


		//where should we go? 0 = continue, 1 = restart ( if in main just restart or do nothing idk), 2 = main menu, 3 = quit to desktop

		if (m_selection == 0)
		{

		}
		if (m_selection == 1)
		{

		}
		if (m_selection == 2)
		{

		}
		if (m_selection == 3)
		{

		}
	}


}

