#include "Character.h"

Character::Character(Mesh* mesh)
	:Object(mesh), key(Backend::GetKeyboard()), velocity()
{
	//Basic
	//maxSpeed = 0.1f;
	//minSpeed = -0.1f;
	//zSpeed = 0.0f;
	//xSpeed = 0.0f;
	//decreaseZSpeed = false;
	//decreaseXSpeed = false;
	//diagMove = false;

	//Movement


	//Jump
	jumpVelocity = 0;
	gravity;
	fallSpeed = -8;
	jumpHeight = 3;
	canJump = false;

	//Boost
	fuel = 10.0f;
	canBoost = false;

	joy.bBox = mesh->bBox;
}

void Character::Move()
{
	float dt = Backend::GetDeltaTime();
	float maxSpeed = 10.0f;
	float speed = 1.0f;
	float counterForce = 0.01f;

	if (key.KeyDown(DIK_W))
		velocity.y += speed;
	else if (velocity.y > 0.f)
		velocity.y -= counterForce;

	if (key.KeyDown(DIK_S))
		velocity.y -= speed;
	else if (velocity.y < 0.f)
		velocity.y += counterForce;

	if (key.KeyDown(DIK_D))
		velocity.x += speed;
	else if (velocity.x > 0.f)
		velocity.x -= counterForce;
	
	if (key.KeyDown(DIK_A))
		velocity.x -= speed;
	else if (velocity.x < 0.f)
		velocity.x += counterForce;

	velocity.x *= 0.99f;
	velocity.y *= 0.99f;


	if (std::abs(velocity.x) > maxSpeed)
		velocity.x *= 0.5;

	if (std::abs(velocity.y) > maxSpeed)
		velocity.y *= 0.5;


	Translate(velocity.x * dt, 0.0f, velocity.y * dt);
	
}

void Character::Jump()
{
	float dt = Backend::GetDeltaTime();

	gravity = 300;
	jumpVelocity -= gravity * dt;

	if (isOnGround)
	{
		jumpVelocity = 0;
		canJump = true;
		canBoost = false;
	}

	//Jump
	if (key.KeyDown(DIK_SPACE)&& canJump)
	{
		canJump = false;
		jumpVelocity += std::sqrtf(2.0f * gravity * jumpHeight);
	}

	if (jumpVelocity < 0 && canBoost == false && fuel > 0)
	{
		canBoost = true;
	}

	//add fuel system ( basicaly remove from fuel variable while boosting )

	//Boost
	if (canBoost && key.KeyDown(DIK_SPACE))
	{
		jumpVelocity += 325 * dt;
	}
	else if (jumpVelocity < -5)
	{
		jumpVelocity = -5;
	}

	this->Translate(0, jumpVelocity * dt, 0);
}

void Character::Respawn()
{
	//Checks if the player has fallen too far off the map and sets the position back to start
	if (GetPosition().y < -10.0f)
		SetPosition(0.0f, 0.0f, 0.0f);
}

void Character::SetSpeedZero()
{
	if (collidedY)
	{
		if (canJump == true)
			this->jumpVelocity = 0.0f;
	}
	//else
	//{
	//	if (xSpeed > 0.001 && zSpeed > 0.001f && xSpeed > zSpeed)
	//		this->xSpeed = 0.0f;
	//	if (zSpeed > 0.001 && xSpeed > 0.001f && zSpeed > xSpeed)
	//		this->zSpeed = 0.0f;
	//	if (xSpeed < -0.001 && zSpeed < -0.001f && xSpeed < zSpeed)
	//		this->xSpeed = 0.0f;
	//	if (zSpeed < -0.001 && xSpeed < -0.001f && zSpeed < xSpeed)
	//		this->zSpeed = 0.0f;
	//	if (xSpeed == 0.0f && zSpeed != 0.0f)
	//		this->zSpeed = 0.0f;
	//	if (xSpeed != 0.0f && zSpeed == 0.0f)
	//		this->xSpeed = 0.0f;
	//}



}

bool Character::SetCanJump(bool canJump)
{
	isOnGround = canJump;
	return isOnGround;
}

bool Character::SetStopMovement(bool stopSpeed)
{
	stopMovement = stopSpeed;
	return stopMovement;
}

bool Character::SetCollidedY(bool verticalCollision)
{
	collidedY = verticalCollision;
	return collidedY;
}





