#pragma once
#include <Windows.h>
#include <d3d11.h>

Backend Backend::system;

class Backend
{
public:

	static bool initiate(HINSTANCE hInst, int showCmd)
	{
		// DO THINGS
	}

	static ID3D11Device* GetDevice()
	{
		return system.dev;
	}

private:

	Backend();

	static Backend system;
	ID3D11Device* dev;
	ID3D11DeviceContext* devCont;

public:

	Backend(const Backend& other) = delete;
	Backend& operator=(const Backend& other) = delete;
	Backend(Backend&& other) = delete;

};