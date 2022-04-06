#include "Transform.h"

Transform::Transform()
    :immutable(false), update(false), position(), rotation(), scale(1.f), matrix4x4()
{
    DirectX::XMStoreFloat4x4(&matrix4x4, DirectX::XMMatrixIdentity());
    Backend::CreateDynamicCBuffer(&worldMatrixBuffer, &matrix4x4, sizeof(DirectX::XMFLOAT4X4));

}

Transform::Transform(DX::XMFLOAT3 pos, DX::XMFLOAT3 rot, FLOAT scale)
    :immutable(true), position(pos), rotation(rot), scale(scale), update(false)
{
    UpdateMatrix();
    Backend::CreateConstCBuffer(&worldMatrixBuffer, &matrix4x4, sizeof(DirectX::XMFLOAT4X4));
}

void Transform::Shutdown()
{
    worldMatrixBuffer->Release();
}

void Transform::Translate(const DX::XMVECTOR& movement)
{
    DX::XMStoreFloat3(&position, DX::XMVectorAdd(DX::XMLoadFloat3(&position), movement));
    update = true;
}

void Transform::Translate(FLOAT X, FLOAT Y, FLOAT Z)
{
    if (immutable)
        return;

    position.x += X;
    position.y += Y;
    position.z += Z;
    update = true;
}

void Transform::SetPosition(const DX::XMVECTOR& Position)
{
    if (immutable)
        return;

    DX::XMStoreFloat3(&position, Position);
    update = true;
}

void Transform::SetPosition(FLOAT X, FLOAT Y, FLOAT Z)
{
    if (immutable)
        return;

    position.x = X;
    position.y = Y;
    position.z = Z;
    update = true;
}

void Transform::Rotate(FLOAT X, FLOAT Y, FLOAT Z)
{
    if (immutable)
        return;

    rotation.x += X;
    rotation.y += Y;
    rotation.z += Z;
    update = true;
}

void Transform::SetRotation(FLOAT X, FLOAT Y, FLOAT Z)
{
    if (immutable)
        return;

    rotation.x = X;
    rotation.y = Y;
    rotation.z = Z;
    update = true;
}

void Transform::Scale(FLOAT amount)
{
    if (immutable)
        return;

    scale += amount;
    update = true;
}

void Transform::SetScale(FLOAT Scale)
{
    if (immutable)
        return;

    scale = Scale;
    update = true;
}

const DX::XMFLOAT3& Transform::GetPosition() const
{
    return position;

}

const DX::XMFLOAT3& Transform::GetRotation() const
{
    return rotation;
}

FLOAT Transform::GetScale() const
{
    return scale;
}

ID3D11Buffer* const* Transform::GetTransformBuffer() const
{
    if (immutable)
        return &worldMatrixBuffer;

    if (update)
        UpdateMatrix();
    
    update = FAILED(Backend::UpdateBuffer(worldMatrixBuffer, &matrix4x4, sizeof(DX::XMFLOAT4X4)));

    return &worldMatrixBuffer;
}

const DX::XMFLOAT4X4& Transform::GetWorldMatrix() const
{
    if (immutable)
        return matrix4x4;

    if (update)
        UpdateMatrix();

    return matrix4x4;
}

void Transform::UpdateMatrix() const
{
    DX::XMStoreFloat4x4(&matrix4x4, DX::XMMatrixTranspose(
        DX::XMMatrixScaling(scale, scale, scale) *
        DX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) *
        DX::XMMatrixTranslation(position.x, position.y, position.z)
    ));
}
