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
	static Backend& Create(HINSTANCE hInst, int showCmd, UINT width, UINT height);

	static void Shutdown();

	static void Process();
	
	static ID3D11Device* GetDevice();
	static ID3D11DeviceContext* GetDeviceContext();
	static IDXGISwapChain* GetSwapChain();
	static ID3D11RenderTargetView* const* GetBackBufferRTV();
	static void Clear();
	static void Display();

	static Window& GetWindow();
	static Mouse& GetMouse();
	static Keyboard& GetKeyboard();
	
	static UINT GetWindowWidth();
	static UINT GetWindowHeight();

	static const D3D11_VIEWPORT& GetStdViewport();

	static FLOAT GetDeltaTime();

	// --- Create Functions ---
	static const std::string ShaderPath;
	static bool LoadShader(const std::string& path, std::string* const outData);

	// Buffers
	static HRESULT CreateConstCBuffer(ID3D11Buffer** buffer, void* Data, UINT byteWidth);
	static HRESULT CreateDynamicCBuffer(ID3D11Buffer** buffer, void* Data, UINT byteWidth);
	static HRESULT UpdateBuffer(ID3D11Buffer* buffer, void* Data, UINT byteWidth);
	static HRESULT CreateVertexBuffer(ID3D11Buffer** buffer, void* Data, UINT byteWidth);

	// Textures
	static HRESULT CreateConstSRVTexture2D(ID3D11Texture2D** texture, void* Data, UINT Width, UINT Height);



private:
	static Backend* systemPtr;
	Backend();

	UINT width, height;
	LPDIRECTINPUT8 DInput;
	Window window;
	Mouse mouse;
	Keyboard keyboard;

	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* bbRTV;
	D3D11_VIEWPORT stdViewport;

	std::chrono::time_point<std::chrono::system_clock> frameStart;
	std::chrono::duration<float> deltaTime;

public:
	Backend(const Backend& other) = delete;
	Backend& operator=(const Backend& other) = delete;
	Backend(Backend&& other) = delete;
	~Backend() = default;
};