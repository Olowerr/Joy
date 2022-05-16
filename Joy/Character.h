#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "Object.h"
#include "Transform.h"
#include <iostream>
#include "Collision.h"
#include "Audio.h"


namespace DX = DirectX;
class Character: public Object
{
public:
	Character(Mesh* mesh);
	~Character();
	void Move();
	void Jump();
	void Respawn();
	bool SetCanJump(bool canJump);


	
private:

	//movement
	Keyboard& key;
	Mesh joy;


	float maxSpeed = 15.0f;
	float speed = 0.2f;
	float counterForce = 0.01f;
	bool stopMovement;
	bool collidedY;


	DirectX::XMFLOAT2 velocity;


	//slide
	DX::XMFLOAT2 slideVel;
	float slideSpeed;
	bool isSliding;
	bool canSlide;
	float timer;

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