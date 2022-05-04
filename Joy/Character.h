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
	bool SetCanJump(bool canJump);


	
private:

	//movement
	Keyboard& key;
	Mesh joy;

	bool stopMovement;
	bool collidedY;


	DirectX::XMFLOAT2 velocity;




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