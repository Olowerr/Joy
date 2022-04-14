#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "Object.h"
#include "Transform.h"
#include <iostream>

namespace DX = DirectX;
class Character: public Object
{
public:
	Character(Mesh* mesh);
	void move();
	void JumpAndBoost();
private:
	Keyboard& key;
	float zSpeed;
	float xSpeed;
	bool diagMove;
	float maxSpeed;
	float minSpeed;
	bool decreaseZSpeed;
	bool decreaseXSpeed;




	float jumpForce, fuel, yPos, gravity, jumpStartPos, minHegihtBeforeBoost, boostAcc, jumpDecc;
	bool isAtMaxHeight, isGrounded, doJump, canBoost;
};