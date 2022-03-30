#pragma once
#include <Windows.h>
#include <d3d11.h>

Backend Backend::system;

class Backend
{
public:

	static bool initiate(HINSTANCE hInst, int showCmd);

	static ID3D11Device* GetDevice();

	static ID3D11DeviceContext* GetDevContext();

	static IDXGISwapChain* GetSwapChain();

private:

	Backend();

	static Backend system;
	ID3D11Device* dev;
	ID3D11DeviceContext* devCont;
	IDXGISwapChain* swapChain;

public:

	Backend(const Backend& other) = delete;
	Backend& operator=(const Backend& other) = delete;
	Backend(Backend&& other) = delete;

};