#include "ImguiSetup.h"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_win32.h"
#include "imgui\imgui_impl_dx11.h"
#include <iostream>

void SetupImGui(HWND windowHandle, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(windowHandle);
	ImGui_ImplDX11_Init(device, deviceContext);
}

void StartImGuiFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiModifyPos(Object* toModify)
{
	DirectX::XMFLOAT3 tempPos = toModify->GetPosition();
	bool begun = ImGui::Begin("Modifiers");
	if (begun)
	{
		ImGui::InputFloat("X pos modifier", &tempPos.x, 0.1f);
		ImGui::InputFloat("Y pos modifier", &tempPos.y, 0.1f);
		ImGui::InputFloat("Z pos modifier", &tempPos.z, 0.1f);
	}
	ImGui::End();

	toModify->SetPosition({ tempPos.x, tempPos.y, tempPos.z });
	std::cout << toModify->GetBoundingBox().Center.x;
}

void ImGuiModifyRot(Object* toModify)
{
	DirectX::XMFLOAT3 tempRot = toModify->GetRotation();
	bool begun = ImGui::Begin("Modifiers");
	if (begun)
	{
		ImGui::InputFloat("X rot modifier", &tempRot.x, 0.1f);
		ImGui::InputFloat("Y rot modifier", &tempRot.y, 0.1f);
		ImGui::InputFloat("Z rot modifier", &tempRot.z, 0.1f);
	}
	ImGui::End();

	toModify->SetRotation(tempRot.x, tempRot.y, tempRot.z);
	std::cout << toModify->GetBoundingBox().Center.x;
}

void ImGuiModifyScale(Object* toModify)
{
	float tempScale = toModify->GetScale();
	bool begun = ImGui::Begin("Modifiers");
	if (begun)
	{
		ImGui::InputFloat("X scale modifier", &tempScale, 0.1f);
	}
	ImGui::End();

	toModify->SetScale(tempScale);
	std::cout << toModify->GetBoundingBox().Center.x;
}

void EndImGuiFrame()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
