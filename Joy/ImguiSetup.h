#pragma once
#include <Windows.h>
#include <d3d11.h>
#include "Object.h"

struct VertexConstantBuffer
{
	float colourModifier[3] = { 0.0f, 0.0f, 0.0f };
	float padding = 0; // Remember packing rules in constant buffers!
	float positionModifer[3] = { 0.0f, 0.0f, 0.0f };
};

void SetupImGui(HWND windowHandle, ID3D11Device* device, ID3D11DeviceContext* deviceContext);

void StartImGuiFrame();

void ImGuiModifyPos(Object* toModify);
void ImGuiModifyRot(Object* toModify);
void ImGuiModifyScale(Object* toModify);

void EndImGuiFrame();