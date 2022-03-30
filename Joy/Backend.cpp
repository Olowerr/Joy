#include "Backend.h"

ID3D11Device* Backend::GetDevice()
{
    return system.dev;
}

ID3D11DeviceContext* Backend::GetDevContext()
{
    return system.devCont;
}

IDXGISwapChain* Backend::GetSwapChain()
{
    return system.swapChain;
}
