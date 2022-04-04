#pragma once
#include <d3d11.h>
#include <assert.h>
#include <string>
#include <fstream>
#include<chrono>
#include "Window.h"
#include "Mouse.h"
#include "Keyboard.h"

class Backend
{
public:
	static void Initiate(HINSTANCE hInst, int showCmd, UINT width, UINT height);
	static void Shutdown();

	static void Process();
	
	static ID3D11Device* GetDevice();
	static ID3D11DeviceContext* GetDeviceContext();
	static IDXGISwapChain* GetSwapChain();
	
	static Window& GetWindow();
	static Mouse& GetMouse();
	static Keyboard& GetKeyboard();
	
	static UINT GetWindowWidth();
	static UINT GetWindowHeight();

	static FLOAT GetDeltaTime();

	static bool LoadShader(const std::string& path, std::string* const outData);
	static bool CreateConstCBuffer(ID3D11Buffer** buffer, void* Data, UINT byteWidth);
	static bool CreateDynamicCBuffer(ID3D11Buffer** buffer, void* Data, UINT byteWidth);
	static bool UpdateBuffer(ID3D11Buffer* buffer, void* Data, UINT byteWidth);

	static const std::string ShaderPath;

private:
	static Backend system;
	Backend();

	UINT width, height;
	LPDIRECTINPUT8 DInput;
	Window window;
	Mouse mouse;
	Keyboard keyboard;

	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain* swapChain;

	std::chrono::time_point<std::chrono::system_clock> frameStart;
	std::chrono::duration<float> deltaTime;

public:
	Backend(const Backend& other) = delete;
	Backend& operator=(const Backend& other) = delete;
	Backend(Backend&& other) = delete;
	~Backend() = default;
};