#pragma once
#include "CameraBase.h"

class FreelookCamera : public Camera
{
public:
	FreelookCamera();
	~FreelookCamera();

	virtual void UpdateCam() override;
	virtual void SetView() override;

private:
	Keyboard& keyboard;
	Mouse& mouse;

	DirectX::XMMATRIX rotMatrix;
	DirectX::XMVECTOR lookDir;
	DirectX::XMVECTOR upDir;
	DirectX::XMVECTOR sideDir;

	const float moveSpeed;
	const float turnSpeed;
	const float aspectRatio;
};