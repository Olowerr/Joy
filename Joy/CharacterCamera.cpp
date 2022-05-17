#include "CharacterCamera.h"

CharacterCamera::CharacterCamera(const Character& object) //const Character& object?
	:object(object), rotation()
{
	position = { 0.0f, 10.0f, -38.0f, 0.0f };
	DirectX::XMStoreFloat3(&positionFloat3, position);
	camFront = { 0.0f, 0.0f, 1.0f, 1.0f };
	camUpDir = { 0.0f, 1.0f, 0.0f, 1.0f };
	rotation = { 0,0,0,0 };
	DirectX::XMMATRIX temp = DirectX::XMMatrixLookAtLH(position, camFront, camUpDir) *
	DirectX::XMMatrixPerspectiveFovLH(0.5f, 2.0f, 0.1f, 500.0f);
	camHeight = 10;
	DirectX::XMStoreFloat4x4(&viewProjMatrix, DirectX::XMMatrixTranspose(temp));

	Backend::CreateDynamicCBuffer(&camMatrixBuffer, &viewProjMatrix, sizeof(DirectX::XMFLOAT4X4));

}

CharacterCamera::~CharacterCamera()
{

}

void CharacterCamera::UpdateCam()
{
	float dt = Backend::GetDeltaTime();
	float x = object.GetPosition().x;
	float z = object.GetPosition().z;
	bool diagonalX = false;
	bool diagonalZ = false;
	float camSpeedX = 3.0f;
	float camSpeedZ = 9.0f;
	float camStillZoneX = 5;

	
	if (true)
	{

	}
	if (x > positionFloat3.x + camStillZoneX)
	{
		diagonalX = true;
		positionFloat3.x += camSpeedX * dt;
	}
	if (x < positionFloat3.x - camStillZoneX)
	{
		diagonalX = true;
		positionFloat3.x -= camSpeedX * dt;
	}
	if (z > positionFloat3.z + 35)
	{
		diagonalZ = true;
		positionFloat3.z += camSpeedZ * dt;
	}
	if (z < positionFloat3.z + 30 )
	{
		diagonalZ = true;
		positionFloat3.z -= (camSpeedZ + 15)* dt;
	}
	positionFloat3.y = object.GetPosition().y + 16;
	camFront = DirectX::XMVectorSet(0.0f, -0.4f, 1.0f, 1.0f);
	position = DirectX::XMLoadFloat3(&positionFloat3);

	DirectX::XMVECTOR positionChange = DirectX::XMVector3Rotate(DirectX::XMVectorSet(direction, 0.f, 0.f, 0.f), DirectX::XMLoadFloat4(&rotation));
	DirectX::XMStoreFloat3(&positionFloat3, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&positionFloat3), positionChange));
}

void CharacterCamera::SetView()
{

	//	camFront = DirectX::XMVectorSet( object.GetPosition().x, object.GetPosition().y, object.GetPosition().z, 1);
	
	//	DirectX::XMVECTOR direction = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0, 0, 1, 0), DirectX::XMLoadFloat4(&rotation));
	DirectX::XMMATRIX viewAndProj = DirectX::XMMatrixLookToLH(position, camFront, camUpDir) * DirectX::XMMatrixPerspectiveFovLH(0.5f, 2.0f, 0.1f, 500.0f);
	XMStoreFloat4x4(&viewProjMatrix, XMMatrixTranspose(viewAndProj));

	Backend::UpdateBuffer(camMatrixBuffer, &viewProjMatrix, 64);

}
