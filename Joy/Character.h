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
	void move();
	void Jump();
	void JumpAndBoost();
	void respawn();
	void setSpeedZero();
	void charGrounded();
private:
	Keyboard& key;
	Mesh joy;
	float zSpeed;
	float xSpeed;
	bool diagMove;
	float maxSpeed;
	float minSpeed;
	bool decreaseZSpeed;
	bool decreaseXSpeed;


	float jumpVelocity;
	float jumpDecc;
	float gravity;
	float jumpHeight;
	float currentYPos;
	float maxJumpHeight;
	float jumpStartHeight;
	bool canJump;
	bool isJumping;
	bool isFalling;


	//float jumpForce, fuel, yPos, groundYPos, gravity, jumpStartPos, minHegihtBeforeBoost, boostAcc, jumpDecc;
	//bool isAtMaxHeight, isGrounded, doJump, canBoost;
};