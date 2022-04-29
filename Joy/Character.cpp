#include "Character.h"

Character::Character(Mesh* mesh)
	:Object(mesh), key(Backend::GetKeyboard())
{
	//Basic
	maxSpeed = 0.1f;
	minSpeed = -0.1f;
	zSpeed = 0.0f;
	xSpeed = 0.0f;
	decreaseZSpeed = false;
	decreaseXSpeed = false;
	diagMove = false;

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

	//After a movement key is pressed the speed in each respective direction is increased.
	//On release a bool is triggered which starts to slow down the player
	if (key.KeyDown(DIK_W) && zSpeed >= -0.001f)
	{
		decreaseZSpeed = false;
		if (zSpeed < maxSpeed)
			zSpeed += 0.06f * Backend::GetDeltaTime();
	}

	if (key.KeyReleased(DIK_W))
	{
		decreaseZSpeed = true;
	}

	if (key.KeyDown(DIK_A) && xSpeed <= 0.001f)
	{
		decreaseXSpeed = false;
		if (xSpeed > minSpeed)
			xSpeed -= 0.06f * Backend::GetDeltaTime();
	}

	if (key.KeyReleased(DIK_A))
	{
		decreaseXSpeed = true;
	}

	if (key.KeyDown(DIK_S) && zSpeed <= 0.001f)
	{
		decreaseZSpeed = false;
		if (zSpeed > minSpeed)
			zSpeed -= 0.06f * Backend::GetDeltaTime();
	}

	if (key.KeyReleased(DIK_S))
	{
		decreaseZSpeed = true;
	}

	if (key.KeyDown(DIK_D) && xSpeed >= -0.001f)
	{
		decreaseXSpeed = false;
		if (xSpeed < maxSpeed)
			xSpeed += 0.06f * Backend::GetDeltaTime();
	}

	if (key.KeyReleased(DIK_D))
	{
		decreaseXSpeed = true;
	}

	//Moves the character and makes sure the diagonal speen can't exceed forward or sideways speed
	if (diagMove == false)
		Translate(xSpeed, 0.0f, zSpeed);

	if (zSpeed != 0.0f && xSpeed != 0.0f)
	{
		diagMove = true;
		Translate(xSpeed / 2, 0.0f, zSpeed / 2);
	}
	else
		diagMove = false;

	//Decreases the speed until the speed has almost reached 0 
	//which results in the speed being set to 0 to properly stop it
	if (decreaseZSpeed == true)
	{
		if (zSpeed > 0.0f)
			zSpeed -= 0.1f * Backend::GetDeltaTime();
		if (zSpeed < 0.0f)
			zSpeed += 0.1f * Backend::GetDeltaTime();
		if (zSpeed < 0.001f && zSpeed> 0.0f)
		{
			decreaseZSpeed = false;
			zSpeed = 0.0f;
		}
		else if (zSpeed > -0.001f && zSpeed < 0.0f)
		{
			decreaseZSpeed = false;
			zSpeed = 0.0f;
		}
	}
	if (decreaseXSpeed == true)
	{
		if (xSpeed > 0.0f)
			xSpeed -= 0.1f * dt;
		if (xSpeed < 0.0f)
			xSpeed += 0.1f * Backend::GetDeltaTime();

		if (xSpeed < 0.001f && xSpeed> 0.0f)
		{
			decreaseXSpeed = false;
			xSpeed = 0.0f;
		}
		else if (xSpeed > -0.001f && xSpeed < 0.0f)
		{
			decreaseXSpeed = false;
			xSpeed = 0.0f;
		}
	}
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
		jumpVelocity += 350 * dt;
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

void Character::setSpeedZero()
{
	if (collidedY)
	{
		if (canJump == true)
			this->jumpVelocity = 0.0f;
	}
	else
	{
		if (xSpeed > 0.001 && zSpeed > 0.001f && xSpeed > zSpeed)
			this->xSpeed = 0.0f;
		if (zSpeed > 0.001 && xSpeed > 0.001f && zSpeed > xSpeed)
			this->zSpeed = 0.0f;
		if (xSpeed < -0.001 && zSpeed < -0.001f && xSpeed < zSpeed)
			this->xSpeed = 0.0f;
		if (zSpeed < -0.001 && xSpeed < -0.001f && zSpeed < xSpeed)
			this->zSpeed = 0.0f;
		if (xSpeed == 0.0f && zSpeed != 0.0f)
			this->zSpeed = 0.0f;
		if (xSpeed != 0.0f && zSpeed == 0.0f)
			this->xSpeed = 0.0f;
	}
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

bool Character::setCollidedY(bool verticalCollision)
{
	collidedY = verticalCollision;
	return collidedY;
}





