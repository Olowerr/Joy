#pragma once
#include <d3d11.h>
#include <assert.h>
#include <string>
#include <fstream>
#include<chrono>
#include "Window.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "ShaderStorage.h"

#include "stb_image.h" // "shouldn't" be here but solves lnk problems

class Backend
{
public:
	static Backend& Create(HINSTANCE hInst, int showCmd, UINT width, UINT height);

	static void Destroy();

	static void Process();
	
	static ID3D11Device* GetDevice();
	static ID3D11DeviceContext* GetDeviceContext();
	static IDXGISwapChain* GetSwapChain();
	static ID3D11RenderTargetView* const* GetBackBufferRTV();
	static ID3D11DepthStencilView* const* GetStandardDSV();
	static void Clear();
	static void Display();

	static GraphicsStorage& GetShaderStorage();

	static Window& GetWindow();
	static Mouse& GetMouse();
	static Keyboard& GetKeyboard();
	
	static UINT GetWindowWidth();
	static UINT GetWindowHeight();

	static const D3D11_VIEWPORT& GetDefaultViewport();

	static FLOAT GetDeltaTime();
	static void ResetDeltaTime();

#ifdef _DEBUG
	static void ReportLive()
	{
		ID3D11Debug* debugger = nullptr;
		system->device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debugger));
		debugger->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		debugger->Release();
	}
#endif // _DEBUG


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
	static Backend* system;
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
	ID3D11DepthStencilView* standardDSV;
	D3D11_VIEWPORT defaultViewport;

	std::chrono::time_point<std::chrono::system_clock> frameStart;
	std::chrono::duration<float> deltaTime;

	GraphicsStorage storage;
	bool InitiateShaders();

public:
	Backend(const Backend& other) = delete;
	Backend& operator=(const Backend& other) = delete;
	Backend(Backend&& other) = delete;
	~Backend() = default;
};