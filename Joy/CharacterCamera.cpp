#include "CharacterCamera.h"

CharacterCamera::CharacterCamera(const Object& object)
	:object(object)
{
	camPos = { 0.0f, 5.0f, 20.0f, 0.0f };
	camFront = { 0.0f, 0.0f, -1.0f, 1.0f };
	camUpDir = { 0.0f, 1.0f, 0.0f, 1.0f };
	camTarget = { DirectX::XMVectorAdd(camPos, camFront) };

	DirectX::XMMATRIX temp = DirectX::XMMatrixLookToLH(DirectX::XMLoadFloat3(&object.GetPosition()), camTarget, camUpDir) * // Byt ut camPos mot Objects pos
		DirectX::XMMatrixPerspectiveFovLH(0.5f, 2.0f, 0.1f, 500.0f);

	DirectX::XMStoreFloat4x4(&viewProjMtrx, temp);
}

CharacterCamera::~CharacterCamera()
{
}

void CharacterCamera::UpdateCam()
{
}
