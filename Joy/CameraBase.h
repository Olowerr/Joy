#pragma once
#include <DirectXMath.h>
#include "Backend.h"

class Camera
{
public:
	Camera();
	virtual void Shutdown();

	virtual void UpdateCam() = 0;
	virtual void SetView() = 0;

	ID3D11Buffer* const* GetMatrixBuffer() const;

protected:
	DirectX::XMFLOAT4X4 viewProjMatrix;
	ID3D11Buffer* camMatrixBuffer;

};