#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <assert.h>

#include "Window.h"

class Backend
{
public:
	static Backend& Get()
	{
		static Backend system;
		return system;
	}

	bool Initiate(HINSTANCE hInst, int showCmd, UINT width, UINT height);
	void Shutdown();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	IDXGISwapChain* GetSwapChain();

	Window& GetWindow();

private:
	Backend();

	Window window;
	
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain* swapChain;

public:
	Backend(const Backend& other) = delete;
	Backend& operator=(const Backend& other) = delete;
	Backend(Backend&& other) = delete;

};