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

	//Jump/Boost variable initiation
	fuel = 10.0f;
	jumpDecc = 0.0f;
	boostAcc = 0.0f;
	canBoost = false;
	doJump = false;
	gravity = 0.0f;
	isAtMaxHeight = false;
	isGrounded = false;
	jumpForce = 0.0f;
	jumpStartPos = 0.0f;
	minHegihtBeforeBoost = 0.0f;
	yPos = 0.0f;
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

void Character::JumpAndBoost()
{

	minHegihtBeforeBoost = 1;  // how high should you be able to jump before being able to boost (from where joy jumped)
	jumpForce = 0;  // reset jumpforce when not pressed
	yPos = this->GetPosition().y;


	if ((this->GetPosition().y - jumpStartPos) >= minHegihtBeforeBoost) //check if reached height where boost becomes available
	{
		canBoost = true;
	}

	if (yPos > 0)
	{
		jumpForce -= jumpDecc;
	}
	else if (yPos <= 0) // change later to check if colided with ground ( now assumes ground is Ypos 0 )
	{
		canBoost = false;
		isGrounded = true;
	}
	if (key.KeyDown(DIK_SPACE) && isGrounded) // when space is pressed and joy is grounded
	{
		jumpStartPos = this->GetPosition().y;
		doJump = true;
		isGrounded = false;
		jumpDecc = 0;
		jumpForce = 10;
		boostAcc = 0;
	}
	if (doJump && !isGrounded)
	{
		jumpForce = 12;

		if (jumpForce > 0)
		{
			jumpDecc += 0.05;
			jumpForce -= jumpDecc;     //acts as gravity, but the value cahnges over time, so i called it something else
		}
	}

	if (key.KeyDown(DIK_SPACE) && !isGrounded && fuel > 0) // boost ( pressed after reaching min limit)
	{

		jumpForce += 1.f;
		fuel -= 0.001;
		jumpForce += boostAcc;
		boostAcc += 0.1;
	}

	if (key.KeyReleased(DIK_SPACE)) //reset decceleration and boost when space is released
	{
		jumpDecc = 0;
		boostAcc = 0;
	}
	float dt = Backend::GetDeltaTime();
	jumpForce *= dt;
	Translate(0, jumpForce, 0);


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




