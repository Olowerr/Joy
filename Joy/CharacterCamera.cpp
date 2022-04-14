#include "CharacterCamera.h"

CharacterCamera::CharacterCamera(const Character& object) //const Character& object?
	:object(object)
{


	camPos = { 0.0f, 5.0f, 20.0f, 0.0f };
	camFront = { 0.0f, 0.0f, -1.0f, 1.0f };
	camUpDir = { 0.0f, 1.0f, 0.0f, 1.0f };
	camTarget = { DirectX::XMVectorAdd(camPos, camFront) };
	rotation = { 0,0,0,0 };
	DirectX::XMMATRIX temp = DirectX::XMMatrixLookToLH(camPos, camTarget, camUpDir) * // Byt ut camPos mot Objects pos
	DirectX::XMMatrixPerspectiveFovLH(0.5f, 2.0f, 0.1f, 500.0f);

	DirectX::XMStoreFloat4x4(&viewProjMtrx, temp);
}

CharacterCamera::~CharacterCamera()
{
	this->camCb->Release();
}

void CharacterCamera::UpdateCam()
{
	float x = object.GetPosition().x;
//	float x = 1;
	if (x < 0 - stillZone)
	{
		direction += acceleration;
		if (direction > 10)
		{
			direction = 10;
		}
	}
	if (x > 0 + stillZone)
	{
		direction -= acceleration;
		if (direction < 10)
		{
			direction = 10;
		}
	}

	
	DirectX::XMVECTOR positionChange = DirectX::XMVector3Rotate(DirectX::XMVectorSet(direction, 0.f, 0.f, 0.f), DirectX::XMLoadFloat4(&rotation));
	DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&position), positionChange));
}

void CharacterCamera::SetView()
{
	
	DirectX::XMVECTOR direction = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0, 0, 1, 0), DirectX::XMLoadFloat4(&rotation));
	DirectX::XMMATRIX viewAndProj = DirectX::XMMatrixLookToLH(XMLoadFloat3(&position), direction, DirectX::XMVectorSet(0, 1, 0, 0)) * DirectX::XMMatrixPerspectiveFovLH(0.5f, 2.0f, 0.1f, 500.0f);
	XMStoreFloat4x4(&viewProjMtrx, XMMatrixTranspose(viewAndProj));

}
