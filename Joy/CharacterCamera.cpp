#include "CharacterCamera.h"

CharacterCamera::CharacterCamera(const Character& object) //const Character& object?
	:object(object), rotation()
{


	camPos = { 0.0f, 10.0f, -20.0f, 0.0f };
	DirectX::XMStoreFloat3(&position, camPos);
	camFront = { 0.0f, 0.0f, 1.0f, 1.0f };
	camUpDir = { 0.0f, 1.0f, 0.0f, 1.0f };
	rotation = { 0,0,0,0 };
	DirectX::XMMATRIX temp = DirectX::XMMatrixLookAtLH(camPos, camFront, camUpDir) *
	DirectX::XMMatrixPerspectiveFovLH(0.5f, 2.0f, 0.1f, 500.0f);
	camHeight = 0;
	DirectX::XMStoreFloat4x4(&viewProjMtrx, temp);
}

CharacterCamera::~CharacterCamera()
{

}

void CharacterCamera::UpdateCam()
{
//	float dt = Backend::GetDeltaTime();
	float dt = 1;
	float x = object.GetPosition().x;

	if (x > position.x +1)
	{
		position.x += 0.009 *dt;
	}
	if (x < position.x -1)
	{
		position.x -= 0.009 *dt;
	}

	
	DirectX::XMVECTOR positionChange = DirectX::XMVector3Rotate(DirectX::XMVectorSet(direction, 0.f, 0.f, 0.f), DirectX::XMLoadFloat4(&rotation));
	DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&position), positionChange));
}

void CharacterCamera::SetView()
{

//	camFront = DirectX::XMVectorSet( object.GetPosition().x, object.GetPosition().y, object.GetPosition().z, 1);
	position.y = object.GetPosition().y * -1 * 0.01 + camHeight;
	camFront = DirectX::XMVectorSet(position.x, object.GetPosition().y, 1, 1);

	
//	DirectX::XMVECTOR direction = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0, 0, 1, 0), DirectX::XMLoadFloat4(&rotation));
	DirectX::XMMATRIX viewAndProj = DirectX::XMMatrixLookAtLH(XMLoadFloat3(&position), camFront, camUpDir) * DirectX::XMMatrixPerspectiveFovLH(0.5f, 2.0f, 0.1f, 500.0f);
	XMStoreFloat4x4(&viewProjMtrx, XMMatrixTranspose(viewAndProj));

}
