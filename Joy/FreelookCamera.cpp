#include "FreelookCamera.h"

FreelookCamera::FreelookCamera()
	:keyboard(Backend::GetKeyboard()), mouse(Backend::GetMouse())
	, aspectRatio((float)Backend::GetWindowWidth() / (float)Backend::GetWindowHeight())
	, rotMatrix(DirectX::XMMatrixIdentity())
	, moveSpeed(2.f), turnSpeed(1.5f)
{

	position = DirectX::XMVectorSet(0.f, 5.f, -5.f, 0.f);
	lookDir = DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f);
	upDir =	DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f);
	sideDir = DirectX::XMVectorSet(1.f, 0.f, 0.f, 0.f);

	DirectX::XMStoreFloat4x4(&viewProjMatrix, DirectX::XMMatrixTranspose(
		DirectX::XMMatrixLookToLH(position, lookDir, upDir) * DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, aspectRatio, 0.1f, 200.f)));

	Backend::CreateDynamicCBuffer(&camMatrixBuffer, &viewProjMatrix, sizeof(DirectX::XMFLOAT4X4));
}

void FreelookCamera::UpdateCam()
{

	const float DeltaTime = Backend::GetDeltaTime();
	const INT MouseX = mouse.GetDeltaX();
	const INT MouseY = mouse.GetDeltaY();

	if (MouseX)
	{
		rotMatrix = DirectX::XMMatrixRotationAxis(upDir, turnSpeed * MouseX * DeltaTime);
		lookDir = DirectX::XMVector3Transform(lookDir, rotMatrix);
		upDir = DirectX::XMVector3Transform(upDir, rotMatrix);
		sideDir = DirectX::XMVector3Cross(lookDir, upDir);
	}

	if (MouseY)
	{
		rotMatrix = DirectX::XMMatrixRotationAxis(sideDir, turnSpeed * MouseY * DeltaTime);
		lookDir = DirectX::XMVector3Transform(lookDir, rotMatrix);
		sideDir = DirectX::XMVector3Transform(sideDir, rotMatrix);
		upDir = DirectX::XMVector3Cross(lookDir, sideDir);
	}

	if (keyboard.KeyDown(DIK_W))
	{
		position = DirectX::XMVectorAdd(position, DirectX::XMVectorScale(lookDir, moveSpeed * DeltaTime));
	}
	else if (keyboard.KeyDown(DIK_S))
	{
		position = DirectX::XMVectorAdd(position, DirectX::XMVectorScale(lookDir, -moveSpeed * DeltaTime));
	}

	if (keyboard.KeyDown(DIK_A))
	{
		position = DirectX::XMVectorAdd(position, DirectX::XMVectorScale(sideDir, -moveSpeed * DeltaTime));
	}
	else if (keyboard.KeyDown(DIK_D))
	{
		position = DirectX::XMVectorAdd(position, DirectX::XMVectorScale(sideDir, moveSpeed * DeltaTime));
	}
}

void FreelookCamera::SetView()
{
	DirectX::XMStoreFloat4x4(&viewProjMatrix, DirectX::XMMatrixTranspose(
		DirectX::XMMatrixLookToLH(position, lookDir, upDir) * DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, aspectRatio, 0.1f, 200.f)));

	Backend::UpdateBuffer(camMatrixBuffer, &viewProjMatrix, 64);
}
