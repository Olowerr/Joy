#pragma once
#include <Windows.h>
#include <d3d11.h>
#include "Object.h"
#include "MapSections.h"
#include "ObjectHelper.h"

#include "imgui\imgui.h"
#include "imgui\imgui_impl_win32.h"
#include "imgui\imgui_impl_dx11.h"

void SetupImGui(HWND windowHandle, ID3D11Device* device, ID3D11DeviceContext* deviceContext);

void StartImGuiFrame();

void ImGuiModifyTransform(std::vector<Object*> object, Camera* camera);
static int imguiObjCounter = 0;

void EndImGuiFrame();