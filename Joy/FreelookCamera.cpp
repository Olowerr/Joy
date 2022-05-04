#include "FreelookCamera.h"
#include <iostream>
FreelookCamera::FreelookCamera()
	:keyboard(Backend::GetKeyboard()), mouse(Backend::GetMouse())
	, aspectRatio((float)Backend::GetWindowWidth() / (float)Backend::GetWindowHeight())
	, rotMatrix(DirectX::XMMatrixIdentity())
	, moveSpeed(2.f), turnSpeed(1.5f)
{

	position = DirectX::XMVectorSet(0.f, 5.f, -5.f, 1.f);
	lookDir = DirectX::XMVectorSet(0.f, 0.f, 1.f, 1.f);
	upDir =	DirectX::XMVectorSet(0.f, 1.f, 0.f, 1.f);
	sideDir = DirectX::XMVector3Cross(lookDir, upDir);

	DirectX::XMStoreFloat4x4(&viewProjMatrix, DirectX::XMMatrixTranspose(
		DirectX::XMMatrixLookToLH(position, lookDir, upDir) * DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, aspectRatio, 0.1f, 200.f)));

	Backend::CreateDynamicCBuffer(&camMatrixBuffer, &viewProjMatrix, sizeof(DirectX::XMFLOAT4X4));
}

FreelookCamera::~FreelookCamera()
{
}

void FreelookCamera::UpdateCam()
{
	return;

	const float DeltaTime = Backend::GetDeltaTime();
	const INT MouseX = mouse.GetDeltaX();
	const INT MouseY = mouse.GetDeltaY();

	using namespace DirectX;

	//x
	rotMatrix = XMMatrixRotationY(MouseX * DeltaTime * turnSpeed);
	lookDir = XMVector3Transform(lookDir, rotMatrix);
	sideDir = XMVector3Transform(sideDir, rotMatrix);

	//y
	lookDir = XMVector3Transform(lookDir, XMMatrixRotationAxis(sideDir, -MouseY * DeltaTime * turnSpeed));
	upDir = XMVector3Cross(sideDir, lookDir);
	


	if (keyboard.KeyDown(DIK_W))
		position = XMVectorAdd(XMVectorScale(lookDir, DeltaTime * moveSpeed), position);
	else if (keyboard.KeyDown(DIK_S))
		position = XMVectorAdd(XMVectorScale(lookDir, DeltaTime * -moveSpeed), position);

	if (keyboard.KeyDown(DIK_D))
		position = XMVectorAdd(XMVectorScale(sideDir, DeltaTime * -moveSpeed), position);
	else if (keyboard.KeyDown(DIK_A))
		position = XMVectorAdd(XMVectorScale(sideDir, DeltaTime * moveSpeed), position);

	if (keyboard.KeyDown(DIK_SPACE))
		position.m128_f32[1] += moveSpeed * DeltaTime;
	if (keyboard.KeyDown(DIK_LSHIFT))
		position.m128_f32[1] -= moveSpeed * DeltaTime;
}

void FreelookCamera::SetView()
{
	DirectX::XMStoreFloat4x4(&viewProjMatrix, DirectX::XMMatrixTranspose(
		DirectX::XMMatrixLookToLH(position, lookDir, upDir) * DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, aspectRatio, 0.1f, 200.f)));

	Backend::UpdateBuffer(camMatrixBuffer, &viewProjMatrix, 64);
}
