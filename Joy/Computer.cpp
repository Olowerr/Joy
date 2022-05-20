#include "Computer.h"

HelpComputer::HelpComputer(Mesh* mesh, const Character& joy)
	:Object(mesh, true), joy(joy), timePassed(0.f)
{
	trigger.Center.x = -10.f;
	trigger.Center.y = 3.1f;
	trigger.Center.z = 1.2f;

	trigger.Extents.x = 2.6f;
	trigger.Extents.y = 2.2f;
	trigger.Extents.z = 3.6f;

	SetPosition(trigger.Center.x - 1.8f, trigger.Center.y + 6.f, trigger.Center.z + 1.2f);
	SetScale(0.5f);
	SetRotation(0.f, 0.7f, 0.f);
}

bool HelpComputer::Check()
{
	timePassed += Backend::GetDeltaTime() * 1.01f;

	auto& pos = GetPosition();
	SetPosition(pos.x, pos.y + std::sin(timePassed) * 0.001f, pos.z);

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

	return joy.GetBoundingBox(0).Intersects(trigger);
}

void HelpComputer::SetTrigger(DirectX::XMFLOAT3 center, DirectX::XMFLOAT3 extents)
{
	trigger.Center = center;
	trigger.Extents = extents;
}
