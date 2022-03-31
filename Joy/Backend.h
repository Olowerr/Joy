#pragma once
#include <d3d11.h>
#include <assert.h>
#include <string>
#include <fstream>

#include "Window.h"
#include "Mouse.h"
#include "Keyboard.h"

class Backend
{
public:
	static Backend& Get()
	{
		static Backend system;
		return system;
	}

	void Initiate(HINSTANCE hInst, int showCmd, UINT width, UINT height);
	void Shutdown();

	void Process();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	IDXGISwapChain* GetSwapChain();

	Window& GetWindow();
	Mouse& GetMouse();
	Keyboard& GetKeyboard();

	UINT GetWindowWidth() const;
	UINT GetWindowHeight() const;

	bool LoadShader(std::string& inPathOutData);

private:
	Backend();

	UINT width, height;
	LPDIRECTINPUT8 DInput;
	Window window;
	Mouse mouse;
	Keyboard keyboard;

	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain* swapChain;

public:
	Backend(const Backend& other) = delete;
	Backend& operator=(const Backend& other) = delete;
	Backend(Backend&& other) = delete;
	~Backend() = default;
};