#pragma once
#include <Windows.h>
#include <d3d11.h>
#include "Object.h"
#include "MapSections.h"
#include "ObjectHelper.h"

void SetupImGui(HWND windowHandle, ID3D11Device* device, ID3D11DeviceContext* deviceContext);

void StartImGuiFrame();

void ImGuiModifyTransform(std::vector<Object*> object, Camera* camera);
static int imguiObjCounter = 0;

void EndImGuiFrame();