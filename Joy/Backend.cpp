#include "Backend.h"
#include<iostream>

#ifdef _DEBUG
    const std::string Backend::ShaderPath = "../Debug/";
#else
    const std::string Backend::ShaderPath = "../Release/";
#endif

Backend::Backend()
    :mouse(window), keyboard(window), DInput(nullptr)
    , device(nullptr), deviceContext(nullptr), swapChain(nullptr)
    , width(0), height(0)
{
}

void Backend::Initiate(HINSTANCE hInst, int showCmd, UINT width, UINT height)
{
    this->width = width;
    this->height = height;

    bool result = window.Initiate(hInst, showCmd, width, height);
    assert(result);
    
    UINT flags = 0;

#ifdef _DEBUG
    flags = D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLvl[] = { D3D_FEATURE_LEVEL_11_0 };
    DXGI_SWAP_CHAIN_DESC swapDesc = {};
    swapDesc.BufferDesc.Width = width;
    swapDesc.BufferDesc.Height = height;
    swapDesc.BufferDesc.RefreshRate.Numerator = 0;
    swapDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapDesc.SampleDesc.Count = 1;
    swapDesc.SampleDesc.Quality = 0;
    swapDesc.BufferUsage = DXGI_USAGE_UNORDERED_ACCESS | DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapDesc.BufferCount = 1;
    swapDesc.OutputWindow = window.GetHWND();
    swapDesc.Windowed = true;
    swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapDesc.Flags = 0;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, featureLvl, 1, D3D11_SDK_VERSION,
        &swapDesc, &swapChain, &device, nullptr, &deviceContext);
    assert(!FAILED(hr));


    hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DInput, NULL);
    assert(!FAILED(hr));

    result = mouse.Initiate(DInput);
    assert(result);

    result = keyboard.Initiate(DInput, hInst);
    assert(result);
    
#ifndef _DEBUG
    swapChain->SetFullscreenState(TRUE, nullptr);
#endif // _DEBUG

    frameStart = std::chrono::system_clock::now();

}

void Backend::Shutdown()
{
    swapChain->Release();
    deviceContext->Release();

#ifdef _DEBUG
    ID3D11Debug* debugger = nullptr;
    device->QueryInterface(__uuidof(ID3D11Debug), (void**)&debugger);
    debugger->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
    debugger->Release();
#endif

    device->Release();
}

void Backend::Process()
{
    deltaTime = std::chrono::system_clock::now() - frameStart;
    frameStart = std::chrono::system_clock::now();

    window.ProcessMessages();

   

    mouse.ReadEvents();
    keyboard.ReadEvents();


    if (!window.IsActive())
        mouse.Lock(false);

    std::cout << 1.f / deltaTime.count()<<"\n";

    //delta time
}

ID3D11Device* Backend::GetDevice()
{
    return device;
}

ID3D11DeviceContext* Backend::GetDeviceContext()
{
    return deviceContext;
}

IDXGISwapChain* Backend::GetSwapChain()
{
    return swapChain;
}

Window& Backend::GetWindow()
{
    return window;
}

Mouse& Backend::GetMouse()
{
    return mouse;
}

Keyboard& Backend::GetKeyboard()
{
    return keyboard;
}

UINT Backend::GetWindowWidth() const
{
    return width;
}

UINT Backend::GetWindowHeight() const
{
    return height;
}

bool Backend::LoadShader(const std::string& path, std::string* const outData)
{
    std::ifstream reader;
    reader.open(path, std::ios::binary);
    if (!reader.is_open())
        return false;

    reader.seekg(0, std::ios::end);
    outData->reserve((UINT)reader.tellg());
    reader.seekg(0, std::ios::beg);

    outData->assign(std::istreambuf_iterator<char>(reader), std::istreambuf_iterator<char>());
    reader.close();

    return true;
}
