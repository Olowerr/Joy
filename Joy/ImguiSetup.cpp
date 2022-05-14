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

void ImGuiModifyTransform(std::vector<Object*> object)
{
	float xPos = object[imguiObjCounter]->GetPosition().x;
	float yPos = object[imguiObjCounter]->GetPosition().y;
	float zPos = object[imguiObjCounter]->GetPosition().z;
	float xRot = object[imguiObjCounter]->GetRotation().x;
	float yRot = object[imguiObjCounter]->GetRotation().y;
	float zRot = object[imguiObjCounter]->GetRotation().z;
	float scale = object[imguiObjCounter]->GetScale();

	bool begun = ImGui::Begin("Modifiers");
	if (begun)
	{
		ImGui::InputInt("Object to change", &imguiObjCounter, 1);
		if (imguiObjCounter > (int)object.size() - 1)
		{
			imguiObjCounter = object.size() - 1;
		}
		xPos = object[imguiObjCounter]->GetPosition().x;
		yPos = object[imguiObjCounter]->GetPosition().y;
		zPos = object[imguiObjCounter]->GetPosition().z;
		ImGui::InputFloat("X pos modifier", &xPos, 0.1f);
		ImGui::InputFloat("Y pos modifier", &yPos, 0.1f);
		ImGui::InputFloat("Z pos modifier", &zPos, 0.1f);
		xRot = object[imguiObjCounter]->GetRotation().x;
		yRot = object[imguiObjCounter]->GetRotation().y;
		zRot = object[imguiObjCounter]->GetRotation().z;
		ImGui::InputFloat("X rot modifier", &xRot, 0.1f);
		ImGui::InputFloat("Y rot modifier", &yRot, 0.1f);
		ImGui::InputFloat("Z rot modifier", &zRot, 0.1f);
		scale = object[imguiObjCounter]->GetScale();
		ImGui::InputFloat("Scale modifier", &scale, 0.1f);
	}
	ImGui::End();

	object[imguiObjCounter]->SetPosition(xPos, yPos, zPos);
	object[imguiObjCounter]->SetRotation(xRot, yRot, zRot);
	object[imguiObjCounter]->SetScale(scale);
}

void EndImGuiFrame()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
