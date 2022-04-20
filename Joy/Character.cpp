#include "Character.h"

Character::Character(Mesh* mesh)
	:Object(mesh), key(Backend::GetKeyboard())
{
	//Basic movement variable initiation
	maxSpeed = 0.1f;
	minSpeed = -0.1f;
	zSpeed = 0.0f;
	xSpeed = 0.0f;
	decreaseZSpeed = false;
	decreaseXSpeed = false;
	diagMove = false;

	//Basic jump variable initiation
	jumpVelocity = 0;
	gravity;
	jumpHeight = 13;
	canJump = false;

	

	//Jump/Boost variable initiation
	//fuel = 10.0f;
	//jumpDecc = 0.0f;
	//boostAcc = 0.0f;
	//canBoost = false;
	//doJump = false;
	//gravity = 0.0f;
	//isAtMaxHeight = false;
	//isGrounded = false;
	//jumpForce = 0.0f;
	//jumpStartPos = 0.0f;
	//minHegihtBeforeBoost = 0.0f;
	//yPos = 0.0f;
	//


	joy.bBox = mesh->bBox;

}

void Character::move()
{
	//After a movement key is pressed the speed in each respective direction is increased.
	//On release a bool is triggered which starts to slow down the player
	if (key.KeyDown(DIK_W)&& zSpeed >= -0.001f)
	{
		decreaseZSpeed = false;
		if(zSpeed <maxSpeed)
			zSpeed += 0.06f * Backend::GetDeltaTime();
	}
	if (key.KeyReleased(DIK_W))
	{
		decreaseZSpeed = true;
	}


	if (key.KeyDown(DIK_A)&& xSpeed <= 0.001f)
	{
		decreaseXSpeed = false;
		if (xSpeed > minSpeed)
			xSpeed -= 0.06f * Backend::GetDeltaTime();
	}
	if (key.KeyReleased(DIK_A))
	{
		decreaseXSpeed = true;
	}


	if (key.KeyDown(DIK_S)&& zSpeed <= 0.001f)
	{
		decreaseZSpeed = false;
		if (zSpeed > minSpeed)
			zSpeed -= 0.06f * Backend::GetDeltaTime();
	}
	if (key.KeyReleased(DIK_S))
	{
		decreaseZSpeed = true;
	}


	if (key.KeyDown(DIK_D)&& xSpeed >= -0.001f)
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
	if(diagMove == false)
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
		if(zSpeed < 0.0f)
			zSpeed += 0.1f * Backend::GetDeltaTime();
		if (zSpeed < 0.001f&& zSpeed> 0.0f)
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
			xSpeed -= 0.1f * Backend::GetDeltaTime();
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
	fuel = 10;
	bool startClock = false;

	gravity = 300;

	//this should check for collision with ground. Is now just checking y pos for 0
	if (this->GetPosition().y <= -0.3)
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
		jumpStartHeight = this->GetPosition().y;
	}

	if (jumpVelocity < 0 && canBoost == false && fuel > 0)
	{
		canBoost = true;
	}

//add fuel system
	//Boost
	if (canBoost && key.KeyDown(DIK_SPACE))
	{
		jumpVelocity += 0.19;
	}

	jumpVelocity *= 0.9995;
	jumpVelocity -= gravity * dt;

	this->Translate(0, jumpVelocity * dt, 0);
}


void Character::respawn()
{
	//temp
	if(key.KeyDown(DIK_G))
		Translate(0.0f, -0.01f, 0.0f);

	//Checks if the player has fallen too far off the map and sets the position back to start
	if (GetPosition().y < -4.0f)
		SetPosition(0.0f, 0.0f, 0.0f);
}

void Character::setSpeedZero()
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

void Character::charGrounded()
{
	//canJump = true;
	//jumpVelocity = 0.0f;
}





