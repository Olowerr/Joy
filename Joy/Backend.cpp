#include "Backend.h"

Backend::Backend()
    :mouse(window), keyboard(window), device(nullptr), deviceContext(nullptr), swapChain(nullptr), DInput(nullptr)
{
}

void Backend::Initiate(HINSTANCE hInst, int showCmd, UINT width, UINT height)
{
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
    window.ProcessMessages();

    mouse.ReadEvents();
    keyboard.ReadEvents();

    if (!window.IsActive())
        mouse.Lock(false);



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
