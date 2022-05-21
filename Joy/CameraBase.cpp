#include "CameraBase.h"

Camera::Camera()
    :viewProjMatrix(), camMatrixBuffer(nullptr)
{
}

Camera::~Camera()
{
}

void Camera::Shutdown()
{
    camMatrixBuffer->Release();
}

ID3D11Buffer* const* Camera::GetMatrixBuffer() const
{
    return &camMatrixBuffer;
}

DirectX::XMVECTOR Camera::GetPosition()
{
    return position;
}

void Camera::SetPosition(float x, float y, float z)
{
    position = DirectX::XMVectorSet(x, y, z, 1.f);
}
