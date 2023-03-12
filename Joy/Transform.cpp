#include "Transform.h"

Transform::Transform()
    :immutable(false), update(false), position(), rotation(), scale(1.f), matrix4x4()
{
    UpdateMatrix();
    Backend::CreateDynamicCBuffer(&worldMatrixBuffer, &matrix4x4, sizeof(DirectX::XMFLOAT4X4));

}

Transform::Transform(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, FLOAT scale)
    :immutable(true), position(pos), rotation(rot), scale(scale), update(false)
{
    UpdateMatrix();
    Backend::CreateDynamicCBuffer(&worldMatrixBuffer, &matrix4x4, sizeof(DirectX::XMFLOAT4X4));////////////////////
}

void Transform::Shutdown()
{
    worldMatrixBuffer->Release();
}

Transform::~Transform()
{
}

void Transform::Translate(const DirectX::XMVECTOR& movement)
{
    if (immutable)
        return;

    DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&position), movement));
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

void Transform::SetPosition(const DirectX::XMVECTOR& Position)
{
    if (immutable)
        return;

    DirectX::XMStoreFloat3(&position, Position);
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

const DirectX::XMFLOAT3& Transform::GetPosition() const
{
    return position;
}

const DirectX::XMFLOAT3& Transform::GetRotation() const
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
    {
        UpdateMatrix();
        update = FAILED(Backend::UpdateBuffer(worldMatrixBuffer, &matrix4x4, sizeof(DirectX::XMFLOAT4X4)));
    }
    

    return &worldMatrixBuffer;
}

const DirectX::XMFLOAT4X4& Transform::GetWorldMatrix() const
{
    if (immutable)
        return matrix4x4;

    if (update)
    {
        UpdateMatrix();
        update = FAILED(Backend::UpdateBuffer(worldMatrixBuffer, &matrix4x4, sizeof(DirectX::XMFLOAT4X4)));
    }

    return matrix4x4;
}

const DirectX::XMMATRIX& Transform::GetWorldMatrixXM() const
{
    if (immutable)
        return matrixXM;

    if (update)
    {
        UpdateMatrix();
        update = FAILED(Backend::UpdateBuffer(worldMatrixBuffer, &matrix4x4, sizeof(DirectX::XMFLOAT4X4)));
    }

    return matrixXM;
}

bool Transform::GetIsImmutable() const
{
    return immutable;
}

void Transform::ApplyParentTransform(const DirectX::XMMATRIX& parentMatrix)
{
    if (update)
        UpdateMatrix();

    DirectX::XMStoreFloat4x4(&matrix4x4, DirectX::XMMatrixTranspose(matrixXM * parentMatrix));
    Backend::UpdateBuffer(worldMatrixBuffer, &matrix4x4, sizeof(DirectX::XMFLOAT4X4));

    update = false;
}

void Transform::UpdateMatrix() const
{
    matrixXM = DirectX::XMMatrixScaling(scale, scale, scale) *
        DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) *
        DirectX::XMMatrixTranslation(position.x, position.y, position.z);

    DirectX::XMStoreFloat4x4(&matrix4x4, DirectX::XMMatrixTranspose(matrixXM));
}
