#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "Object.h"
#include "Transform.h"
#include <iostream>
#include "Collision.h"


namespace DX = DirectX;
class Character: public Object
{
public:
	Character(Mesh* mesh);
	void Move();
	void Jump();
	void Respawn();
	void setSpeedZero();
//	void charGrounded();
	bool SetCanJump(bool canJump);
	bool SetStopMovement(bool stopSpeed);
	bool setCollidedY(bool verticalCollision);
	
private:

	//movement
	Keyboard& key;
	Mesh joy;
	float zSpeed;
	float xSpeed;
	bool diagMove;
	float maxSpeed;
	float minSpeed;
	bool decreaseZSpeed;
	bool decreaseXSpeed;
	bool stopMovement;
	bool collidedY;

	//jump
	float jumpVelocity;
	float jumpDecc;
	float gravity;
	float jumpHeight;
	float currentYPos;
	float maxJumpHeight;
	float jumpStartHeight;

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