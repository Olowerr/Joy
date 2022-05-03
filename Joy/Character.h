#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "Object.h"
#include "Transform.h"
#include <iostream>
#include "Collision.h"
#include <DirectXMath.h>


namespace DX = DirectX;
class Character: public Object
{
public:
	Character(Mesh* mesh);
	void Move();
	void Slide();
	void Jump();
	void Respawn();
	void SetSpeedZero();
	bool SetCanJump(bool canJump);
	bool SetStopMovement(bool stopSpeed);
	bool SetCollidedY(bool verticalCollision);

	
private:

	//movement
	Keyboard& key;
	Mesh joy;

	float maxSpeed;
	float speed;
	float counterForce;
	bool wsPressed;
	bool adPressed;

	bool stopMovement;
	bool collidedY;


	DirectX::XMFLOAT2 velocity;

	//slide
	DX::XMFLOAT2 slideVel;
	float slideSpeed;
	bool isSliding;
	bool canSlide;

	DX::XMFLOAT2 startSlide;
	DX::XMFLOAT2 endSlide;


	//jump
	float jumpVelocity;
	float jumpDecc;
	float gravity;
	float jumpHeight;
	float currentYPos;
	float maxJumpHeight;
	float jumpStartHeight;
	float timer;
	float fallSpeed;

	bool canJump;
	bool isJumping;
	bool isFalling;
	bool isOnGround;

	//boost
	float boost;
	bool canBoost;
	float fuel;

};