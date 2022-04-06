#pragma once
#include "Backend.h"

#include <DirectXMath.h>

namespace DX = DirectX;

class Transform
{
public:
	Transform();
	Transform(DX::XMFLOAT3 pos, DX::XMFLOAT3 rot, FLOAT scale);
	virtual void Shutdown();

	void Translate(const DX::XMVECTOR& movement);
	void Translate(FLOAT X, FLOAT Y, FLOAT Z);
	void SetPosition(const DX::XMVECTOR& position);
	void SetPosition(FLOAT X, FLOAT Y, FLOAT Z);

	void Rotate(FLOAT X, FLOAT Y, FLOAT Z);
	void SetRotation(FLOAT X, FLOAT Y, FLOAT Z);

	void Scale(FLOAT amount);
	void SetScale(FLOAT Scale);

	const DX::XMFLOAT3& GetPosition() const;
	const DX::XMFLOAT3& GetRotation() const;
	FLOAT GetScale() const;

	ID3D11Buffer* const* GetTransformBuffer() const;
	const DX::XMFLOAT4X4& GetWorldMatrix() const;

private:
	const bool immutable;

	DX::XMFLOAT3 position;
	DX::XMFLOAT3 rotation;
	FLOAT scale;

	mutable DX::XMFLOAT4X4 matrix4x4;

	ID3D11Buffer* worldMatrixBuffer;
	mutable bool update;

	void UpdateMatrix() const;
};