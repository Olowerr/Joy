#include "ImguiSetup.h"

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

void ImGuiModifyTransform(std::vector<Object*> object, Camera* camera)
{
	static bool highLight = true, lines = true;
	static int currentBB = 0;

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
		imguiObjCounter = imguiObjCounter < 0 ? 0 : imguiObjCounter;

		ImGui::Checkbox("HighLight", &highLight);

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


	if (ImGui::Begin("BBox"))
	{
		ImGui::Checkbox("Show BB", &lines);
		if (ImGui::Button("Add BB"))
		{
			object[imguiObjCounter]->AddBBox(object[imguiObjCounter]->GetPosition(), { 1.f, 1.f, 1.f });
			currentBB = object[imguiObjCounter]->GetNumBboxes();
		}

		ImGui::InputInt("Current BB", &currentBB, 1);

		int numBox = object[imguiObjCounter]->GetNumBboxes();
		if (!numBox)
		{
			currentBB = -1;
			goto elgato;
		}
		else
			currentBB = currentBB < 0 ? 0 : currentBB;

		currentBB = currentBB >= numBox ? numBox - 1 : currentBB;

		if (ImGui::Button("Remove BB"))
			object[imguiObjCounter]->RemoveBBox(currentBB);


		DirectX::XMFLOAT3 center = object[imguiObjCounter]->GetBoundingBox(currentBB).Center;
		DirectX::XMFLOAT3 extents = object[imguiObjCounter]->GetBoundingBox(currentBB).Extents;

		ImGui::InputFloat("Center X", &center.x, 0.1f);
		ImGui::InputFloat("Center Y", &center.y, 0.1f);
		ImGui::InputFloat("Center Z", &center.z, 0.1f);
		ImGui::InputFloat("Extent X", &extents.x, 0.1f);
		ImGui::InputFloat("Extent Y", &extents.y, 0.1f);
		ImGui::InputFloat("Extent Z", &extents.z, 0.1f);

		object[imguiObjCounter]->SetBBox(currentBB, center, extents);

		if (ImGui::Button("Print"))
		{
			for (UINT i = 0; i < (UINT)object[imguiObjCounter]->GetNumBboxes(); i++)
			{
				center = object[imguiObjCounter]->GetBoundingBox(i).Center;
				extents = object[imguiObjCounter]->GetBoundingBox(i).Extents;

				printf("\nsceneObjects.at(%d).AddBBox(F3(%.3ff, %.3ff, %.3ff), F3(%.3ff, %.3ff, %.3ff));",
					imguiObjCounter, center.x, center.y, center.z, extents.x, extents.y, extents.z);
			}
		}
	}
	elgato:
	ImGui::End();

	object[imguiObjCounter]->SetPosition(xPos, yPos, zPos);
	object[imguiObjCounter]->SetRotation(xRot, yRot, zRot);
	//object[imguiObjCounter]->SetScale(scale); // idk how to solve

	if (currentBB < 0)
		lines = false;

	HObject::GetInstance().Draw(object[imguiObjCounter], camera, highLight, lines, currentBB);
}

void EndImGuiFrame()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
