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
