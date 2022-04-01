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
	static Backend& Get()
	{
		return system;
	}

	static void Initiate(HINSTANCE hInst, int showCmd, UINT width, UINT height);
	static void Shutdown();

	void Process();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	IDXGISwapChain* GetSwapChain();

	Window& GetWindow();
	Mouse& GetMouse();
	Keyboard& GetKeyboard();

	UINT GetWindowWidth() const;
	UINT GetWindowHeight() const;

	FLOAT GetDeltaTime() const;

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