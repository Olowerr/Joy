#pragma once
#include <DirectXMath.h>
#include "Object.h"
#include "Character.h"


class CharacterCamera
{
public:

	CharacterCamera(const Character& object); 
	~CharacterCamera();
	DirectX::XMFLOAT4X4 GetViewAndProj() { return this->viewProjMtrx; }
//	ID3D11Buffer* GetConstantBuffer() { return this->camCb; } idk if we need this
	void UpdateCam();
	void SetView();

private:
	const Character& object;

	DirectX::XMVECTOR camPos;
	DirectX::XMVECTOR camFront;
	DirectX::XMVECTOR camUpDir;
	DirectX::XMVECTOR camTarget;
	float camHeight;
	DirectX::XMFLOAT4X4 viewProjMtrx;
	ID3D11Buffer* camCb;


	DirectX::XMFLOAT4 rotation;
 	DirectX::XMFLOAT3 position;
	float direction;
	//float direction, stillZone, acceleration, slowStopping;
	//bool isMovingRight, isMovingLeft;
};
