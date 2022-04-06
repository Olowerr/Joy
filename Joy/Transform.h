#pragma once
#include "Backend.h"

#include <DirectXMath.h>

class Transform
{
public:
	Transform();
	Transform(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, FLOAT scale);
	virtual void Shutdown();

	void Translate(const DirectX::XMVECTOR& movement);
	void Translate(FLOAT X, FLOAT Y, FLOAT Z);
	void SetPosition(const DirectX::XMVECTOR& position);
	void SetPosition(FLOAT X, FLOAT Y, FLOAT Z);

	void Rotate(FLOAT X, FLOAT Y, FLOAT Z);
	void SetRotation(FLOAT X, FLOAT Y, FLOAT Z);

	void Scale(FLOAT amount);
	void SetScale(FLOAT Scale);

	const DirectX::XMFLOAT3& GetPosition() const;
	const DirectX::XMFLOAT3& GetRotation() const;
	FLOAT GetScale() const;

	ID3D11Buffer* const* GetTransformBuffer() const;
	const DirectX::XMFLOAT4X4& GetWorldMatrix() const;

private:
	const bool immutable;

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	FLOAT scale;

	mutable DirectX::XMFLOAT4X4 matrix4x4;

	ID3D11Buffer* worldMatrixBuffer;
	mutable bool update;

	void UpdateMatrix() const;
};