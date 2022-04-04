#pragma once
#include <DirectXMath.h>
#include "Object.h"

class CharacterCamera
{
public:

	CharacterCamera(const Object& object);
	~CharacterCamera();

	void UpdateCam();

private:

	DirectX::XMVECTOR camPos;
	DirectX::XMVECTOR camFront;
	DirectX::XMVECTOR camUpDir;
	DirectX::XMVECTOR camTarget;
	DirectX::XMFLOAT4X4 viewProjMtrx;

	const Object& object;
};

