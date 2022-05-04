#pragma once
#include <Windows.h>
#include <d3d11.h>
#include "Object.h"
#include "MapSections.h"

void SetupImGui(HWND windowHandle, ID3D11Device* device, ID3D11DeviceContext* deviceContext);

void StartImGuiFrame();

void ImGuiModifyTransform(std::vector<Object*> object);
static int imguiObjCounter = 0;

void EndImGuiFrame();