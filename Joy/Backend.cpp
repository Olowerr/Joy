#include "Backend.h"
#include<iostream>

Backend Backend::system;

#ifdef _DEBUG
    const std::string Backend::ShaderPath = "../Debug/";
#else
    const std::string Backend::ShaderPath = "../Release/";
#endif

Backend::Backend()
    :mouse(window), keyboard(window), DInput(nullptr)
    , device(nullptr), deviceContext(nullptr), swapChain(nullptr)
    , width(0), height(0), deltaTime(0.f)
{
}

void Backend::Initiate(HINSTANCE hInst, int showCmd, UINT width, UINT height)
{
    //if (system)
    //    return
    //system = new Backend;

    system.width = width;
    system.height = height;

    bool result = system.window.Initiate(hInst, showCmd, width, height);
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
    swapDesc.OutputWindow = system.window.GetHWND();
    swapDesc.Windowed = true;
    swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapDesc.Flags = 0;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, featureLvl, 1, D3D11_SDK_VERSION,
        &swapDesc, &system.swapChain, &system.device, nullptr, &system.deviceContext);
    assert(!FAILED(hr));


    hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&system.DInput, NULL);
    assert(!FAILED(hr));

    result = system.mouse.Initiate(system.DInput);
    assert(result);

    result = system.keyboard.Initiate(system.DInput, hInst);
    assert(result);
    
#ifndef _DEBUG
    system.swapChain->SetFullscreenState(TRUE, nullptr);
#endif // _DEBUG

    system.frameStart = std::chrono::system_clock::now();

}

void Backend::Shutdown()
{
    system.swapChain->Release();
    system.deviceContext->Release();

#ifdef _DEBUG
    ID3D11Debug* debugger = nullptr;
    system.device->QueryInterface(__uuidof(ID3D11Debug), (void**)&debugger);
    debugger->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
    debugger->Release();
#endif

    system.device->Release();

    //delete system;
}

void Backend::Process()
{
    system.deltaTime = std::chrono::system_clock::now() - system.frameStart;
    system.frameStart = std::chrono::system_clock::now();

    system.window.ProcessMessages();

    system.mouse.ReadEvents();
    system.keyboard.ReadEvents();

    if (!system.window.IsActive())
        system.mouse.Lock(false);
}

ID3D11Device* Backend::GetDevice()
{
    return system.device;
}

ID3D11DeviceContext* Backend::GetDeviceContext()
{
    return system.deviceContext;
}

IDXGISwapChain* Backend::GetSwapChain()
{
    return system.swapChain;
}

Window& Backend::GetWindow()
{
    return system.window;
}

Mouse& Backend::GetMouse()
{
    return system.mouse;
}

Keyboard& Backend::GetKeyboard()
{
    return system.keyboard;
}

UINT Backend::GetWindowWidth()
{
    return system.width;
}

UINT Backend::GetWindowHeight()
{
    return system.height;
}

FLOAT Backend::GetDeltaTime()
{
    return system.deltaTime.count();
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

bool Backend::CreateConstCBuffer(ID3D11Buffer** buffer, void* Data, UINT byteWidth)
{
    D3D11_BUFFER_DESC desc{};
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.ByteWidth = byteWidth;
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.CPUAccessFlags = 0;
    desc.StructureByteStride = 0;
    desc.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA inData{};
    inData.pSysMem = Data;
    inData.SysMemPitch = inData.SysMemSlicePitch = 0;
    return SUCCEEDED(system.device->CreateBuffer(&desc, &inData, buffer));
}

bool Backend::CreateDynamicCBuffer(ID3D11Buffer** buffer, void* Data, UINT byteWidth)
{
    D3D11_BUFFER_DESC desc{};
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.ByteWidth = byteWidth;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    desc.StructureByteStride = 0;
    desc.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA inData{};
    inData.pSysMem = Data;
    inData.SysMemPitch = inData.SysMemSlicePitch = 0;
    return SUCCEEDED(system.device->CreateBuffer(&desc, &inData, buffer));
}

bool Backend::UpdateBuffer(ID3D11Buffer* buffer, void* Data, UINT byteWidth)
{
    D3D11_MAPPED_SUBRESOURCE sub;
    if (FAILED(system.deviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub)))
        return false;

    memcpy(sub.pData, Data, byteWidth);
    system.deviceContext->Unmap(buffer, 0);

    return true;
}
