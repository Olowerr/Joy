#pragma once
#include "CameraBase.h"
#include "Object.h"
#include "Character.h"


class CharacterCamera : public Camera
{
public:

	CharacterCamera(const Character& object);
	~CharacterCamera();
	DirectX::XMFLOAT4X4 GetViewAndProj() { return viewProjMatrix; }
	//	ID3D11Buffer* GetConstantBuffer() { return this->camCb; } idk if we need this
	virtual void UpdateCam() override;
	virtual void SetView();

private:
	const Character& object;

	DirectX::XMVECTOR camPos;
	DirectX::XMVECTOR camFront;
	DirectX::XMVECTOR camUpDir;
	DirectX::XMVECTOR camTarget;
	float camHeight;

	DirectX::XMFLOAT4 rotation;
 	DirectX::XMFLOAT3 position;
	float direction;
};
