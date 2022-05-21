#include "Computer.h"

HelpComputer::HelpComputer(Mesh* mesh, const Character& joy, UIRenderer& uiRender)
	:Object(mesh, true), joy(joy), timePassed(0.f), uiRender(uiRender)
	, sprite("../Resources/Images/HowToPlay.png", 0.f, -1.f, 1.f, 1.f)
{
	sprite.SetActive(false);
	uiRender.Add(&sprite);

	trigger.Center.x = -10.f;
	trigger.Center.y = 3.1f;
	trigger.Center.z = 1.2f;

	trigger.Extents.x = 2.6f;
	trigger.Extents.y = 2.2f;
	trigger.Extents.z = 3.6f;

	SetPosition(trigger.Center.x - 1.8f, trigger.Center.y + 6.f, trigger.Center.z + 1.2f);
	SetScale(0.5f);
	SetRotation(0.f, 0.7f, 0.f);

	startPos = GetPosition();
}

/*

can fall through ground when sliding
pick-up text veri fast


*/

void HelpComputer::Shutdown()
{
	Object::Shutdown();

	sprite.SetActive(false);
	sprite.Shutdown();
}

bool HelpComputer::Check()
{
	timePassed += Backend::GetDeltaTime();
	SetPosition(startPos.x, startPos.y + std::sin(timePassed) * 0.5f, startPos.z);

#ifdef _DEBUG
	if (ImGui::Begin("dora"))
	{
		ImGui::InputFloat("CX", &trigger.Center.x, 0.1f);
		ImGui::InputFloat("CY", &trigger.Center.y, 0.1f);
		ImGui::InputFloat("CZ", &trigger.Center.z, 0.1f);

		ImGui::InputFloat("EX", &trigger.Extents.x, 0.1f);
		ImGui::InputFloat("EY", &trigger.Extents.y, 0.1f);
		ImGui::InputFloat("EZ", &trigger.Extents.z, 0.1f);

		ImGui::End();
	}

	HObject::GetInstance().DrawBB(trigger);
#endif

	if ((!showing && joy.GetBoundingBox(0).Intersects(trigger)) && Backend::GetKeyboard().KeyReleased(DIK_E))
	{
		uiRender.DisableAll();
		sprite.SetActive(true);
		showing = true;
	}
	else if ((showing && !joy.GetBoundingBox(0).Intersects(trigger)) || Backend::GetKeyboard().KeyReleased(DIK_E))
	{
		//uiRender.EnableAll();
		sprite.SetActive(false);
		showing = false;
	}

	
	return showing;
}

void HelpComputer::SetTrigger(DirectX::XMFLOAT3 center, DirectX::XMFLOAT3 extents)
{
	trigger.Center = center;
	trigger.Extents = extents;
}

void HelpComputer::SetActive(bool active)
{
	sprite.SetActive(active);
}
