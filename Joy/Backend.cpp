#include "Backend.h"
#include<iostream>

Backend* Backend::system = nullptr;

#ifdef _DEBUG
    const std::string Backend::ShaderPath = "../Debug/";
#else
    const std::string Backend::ShaderPath = "../Release/";
#endif

Backend::Backend()
    :mouse(window), keyboard(window), DInput(nullptr)
    , device(nullptr), deviceContext(nullptr), swapChain(nullptr), bbRTV(nullptr)
    , width(0), height(0), deltaTime(0.f), defaultViewport()
{
}

Backend& Backend::Create(HINSTANCE hInst, int showCmd, UINT width, UINT height)
{
    if (system) // Check if already created
        return *system;

    system = new Backend;

    system->width = width;
    system->height = height;

    bool succeeded = false;
    HRESULT hr;

    succeeded = system->window.Initiate(hInst, showCmd, width, height);
    assert(succeeded);

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
    swapDesc.OutputWindow = system->window.GetHWND();
    swapDesc.Windowed = true;
    swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapDesc.Flags = 0;

    hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, featureLvl, 1, D3D11_SDK_VERSION,
        &swapDesc, &system->swapChain, &system->device, nullptr, &system->deviceContext);
    assert(SUCCEEDED(hr));

    ID3D11Texture2D* backBuffer{};
    hr = system->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
    if (FAILED(hr)) //fixes warning on create RTV
    {
        assert(SUCCEEDED(hr));
        return *system;
    }

    hr = system->device->CreateRenderTargetView(backBuffer, nullptr, &system->bbRTV);
    backBuffer->Release();
    assert(SUCCEEDED(hr));

    succeeded = system->InitiateShaders();
    assert(succeeded);

    D3D11_TEXTURE2D_DESC texDesc{};
    texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    texDesc.ArraySize = 1;
    texDesc.MipLevels = 1;
    texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    texDesc.CPUAccessFlags = 0;
    texDesc.Height = height;
    texDesc.Width = width;
    texDesc.MiscFlags = 0;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    ID3D11Texture2D* resource{};
    hr = system->device->CreateTexture2D(&texDesc, nullptr, &resource);
    if (FAILED(hr))
    {
        assert(SUCCEEDED(hr));
        return *system;
    }
    hr = system->device->CreateDepthStencilView(resource, nullptr, &system->standardDSV);
    resource->Release();
    assert(SUCCEEDED(hr));


    hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&system->DInput), NULL);
    assert(SUCCEEDED(hr));

    succeeded = system->mouse.Initiate(system->DInput);
    assert(succeeded);

    succeeded = system->keyboard.Initiate(system->DInput, hInst);
    assert(succeeded);

#ifndef _DEBUG
    system->swapChain->SetFullscreenState(TRUE, nullptr);
#endif // _DEBUG

    

    system->defaultViewport.TopLeftX = 0;
    system->defaultViewport.TopLeftY = 0;
    system->defaultViewport.Width = (float)width;
    system->defaultViewport.Height = (float)height;
    system->defaultViewport.MinDepth = 0;
    system->defaultViewport.MaxDepth = 1;

    system->frameStart = std::chrono::system_clock::now();

    return *system;
}

void Backend::Destroy()
{
    if (!system)
        return;

    system->storage.Shutdown();
    system->swapChain->Release();
    system->deviceContext->Release();
    system->bbRTV->Release();
    system->standardDSV->Release();

#ifdef _DEBUG
    ID3D11Debug* debugger = nullptr;
    system->device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debugger));
    debugger->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
    debugger->Release();
#endif

    system->device->Release();

    delete system;
    system = nullptr;
}

void Backend::Process()
{
    system->deltaTime = std::chrono::system_clock::now() - system->frameStart;
    system->frameStart = std::chrono::system_clock::now();

    system->window.ProcessMessages();

    system->mouse.ReadEvents();
    system->keyboard.ReadEvents();

    if (!system->window.IsActive())
    {
        system->mouse.Lock(false);
        //ShowWindow(system->window.GetHWND(), SW_MINIMIZE);
    }
}

ID3D11Device* Backend::GetDevice()
{
    return system->device;
}

ID3D11DeviceContext* Backend::GetDeviceContext()
{
    return system->deviceContext;
}

IDXGISwapChain* Backend::GetSwapChain()
{
    return system->swapChain;
}

ID3D11RenderTargetView* const* Backend::GetBackBufferRTV()
{
    return &system->bbRTV;
}

ID3D11DepthStencilView* const* Backend::GetStandardDSV()
{
    return &system->standardDSV;
}

void Backend::Clear()
{
    static const float clearColour[4] = { 0.2f, 0.2f, 0.2f, 0.f };
    system->deviceContext->ClearRenderTargetView(system->bbRTV, clearColour);
    system->deviceContext->ClearDepthStencilView(system->standardDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void Backend::Display()
{
    system->swapChain->Present(0, 0);
}

GraphicsStorage& Backend::GetShaderStorage()
{
    return system->storage;
}

Window& Backend::GetWindow()
{
    return system->window;
}

Mouse& Backend::GetMouse()
{
    return system->mouse;
}

Keyboard& Backend::GetKeyboard()
{
    return system->keyboard;
}

UINT Backend::GetWindowWidth()
{
    return system->width;
}

UINT Backend::GetWindowHeight()
{
    return system->height;
}

const D3D11_VIEWPORT& Backend::GetDefaultViewport()
{
    return system->defaultViewport;
}

FLOAT Backend::GetDeltaTime()
{
    return system->deltaTime.count();
}

void Backend::ResetDeltaTime()
{
    system->deltaTime = std::chrono::duration<float>(0.0f);
}

bool Backend::InitiateShaders()
{
    std::string shaderData;
    bool succeeded = false;
    HRESULT hr;

    D3D11_INPUT_ELEMENT_DESC inputDesc[3] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    // Depth
    succeeded = LoadShader(ShaderPath + "PosOnlyVS.cso", &shaderData);
    if (!succeeded)
        return false;
    
    hr = system->device->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &storage.posOnlyVS);
    if (FAILED(hr))
        return false;

    hr = system->device->CreateInputLayout(inputDesc, 1, shaderData.c_str(), shaderData.length(), &storage.posOnlyInputLayout);
    if (FAILED(hr))
        return false;


    // Object VS
    succeeded = LoadShader(ShaderPath + "ObjVS.cso", &shaderData);
    if (!succeeded)
        return false;

    hr = system->device->CreateInputLayout(inputDesc, 3, shaderData.c_str(), shaderData.length(), &storage.objectInputLayout);
    if (FAILED(hr))
        return false;

    hr = system->device->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &storage.objectVS);
    if (FAILED(hr))
        return false;


    // Object PS
    succeeded = LoadShader(ShaderPath + "ObjPS.cso", &shaderData);
    if (!succeeded)
        return false;
    
    hr = system->device->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &storage.objectPS);
    if (FAILED(hr))
        return false;

    // Object Instanced VS
    succeeded = LoadShader(ShaderPath + "ObjInstanceVS.cso", &shaderData);
    if (!succeeded)
        return false;

    hr = system->device->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &storage.objectInstancedVS);
    if (FAILED(hr))
        return false;


    // Joy
    succeeded = LoadShader(ShaderPath + "JoyPS.cso", &shaderData);
    if (!succeeded)
        return false;

    hr = system->device->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &storage.JoyPS);
    if (FAILED(hr))
        return false;


    return true;

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

HRESULT Backend::CreateConstCBuffer(ID3D11Buffer** buffer, void* Data, UINT byteWidth)
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
    return system->device->CreateBuffer(&desc, &inData, buffer);
}

HRESULT Backend::CreateDynamicCBuffer(ID3D11Buffer** buffer, void* Data, UINT byteWidth)
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

    return Data ? system->device->CreateBuffer(&desc, &inData, buffer) : system->device->CreateBuffer(&desc, nullptr, buffer);
}

HRESULT Backend::UpdateBuffer(ID3D11Buffer* buffer, void* Data, UINT byteWidth)
{
    D3D11_MAPPED_SUBRESOURCE sub;
    HRESULT hr;
    hr = system->deviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
    if (FAILED(hr))
        return hr;

    memcpy(sub.pData, Data, byteWidth);
    system->deviceContext->Unmap(buffer, 0);

    return hr;
}

HRESULT Backend::CreateVertexBuffer(ID3D11Buffer** buffer, void* Data, UINT byteWidth)
{
    D3D11_BUFFER_DESC desc{};
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;;
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.ByteWidth = byteWidth;
    desc.CPUAccessFlags = 0;
    desc.StructureByteStride = 0;
    desc.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA inData{};
    inData.pSysMem = Data;
    inData.SysMemPitch = inData.SysMemSlicePitch = 0;

    return system->device->CreateBuffer(&desc, &inData, buffer);
}

HRESULT Backend::CreateConstSRVTexture2D(ID3D11Texture2D** texture, void* Data, UINT Width, UINT Height)
{
    D3D11_TEXTURE2D_DESC desc{};
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.ArraySize = 1;
    desc.MipLevels = 1;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.Height = Height;
    desc.Width = Width;
    desc.MiscFlags = 0;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    
    D3D11_SUBRESOURCE_DATA inData{};
    inData.pSysMem = Data;
    inData.SysMemPitch = Width * 4;
    inData.SysMemSlicePitch = 0;

    return system->device->CreateTexture2D(&desc, &inData, texture);
}
