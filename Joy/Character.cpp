#include "Character.h"

Character::Character(Mesh* mesh)
	:Object(mesh), key(Backend::GetKeyboard())
{
	maxSpeed = 0.1f;
	minSpeed = -0.1f;
	zSpeed = 0.0f;
	xSpeed = 0.0f;
	decreaseZSpeed = false;
	decreaseXSpeed = false;
	diagMove = false;
}

void Character::move()
{
	//After a movement key is pressed the speed in each respective direction is increased.
	//On release a bool is triggered which starts to slow down the player
	if (key.KeyDown(DIK_W)&& zSpeed >= -0.001f)
	{
		decreaseZSpeed = false;
		if(zSpeed <maxSpeed)
			zSpeed += 0.05f * Backend::GetDeltaTime();
	}
	if (key.KeyReleased(DIK_W))
	{
		decreaseZSpeed = true;
	}


	if (key.KeyDown(DIK_A)&& xSpeed <= 0.001f)
	{
		decreaseXSpeed = false;
		if (xSpeed > minSpeed)
			xSpeed -= 0.05f * Backend::GetDeltaTime();
	}
	if (key.KeyReleased(DIK_A))
	{
		decreaseXSpeed = true;
	}


	if (key.KeyDown(DIK_S)&& zSpeed <= 0.001f)
	{
		decreaseZSpeed = false;
		if (zSpeed > minSpeed)
			zSpeed -= 0.05f * Backend::GetDeltaTime();
	}
	if (key.KeyReleased(DIK_S))
	{
		decreaseZSpeed = true;
	}


	if (key.KeyDown(DIK_D)&& xSpeed >= -0.001f)
	{
		decreaseXSpeed = false;
		if (xSpeed < maxSpeed)
			xSpeed += 0.05f * Backend::GetDeltaTime();
	}
	if (key.KeyReleased(DIK_D))
	{
		decreaseXSpeed = true;
	}


	//Flyttar karaktären och ser till att diagonal rörelse inte är snabbare än rakt fram/bak
	if(diagMove == false)
		Translate(xSpeed, 0.0f, zSpeed);
	if (zSpeed != 0.0f && xSpeed != 0.0f)
	{
		diagMove = true;
		Translate(xSpeed / 2, 0.0f, zSpeed / 2);
	}
	else
		diagMove = false;
		

	//Minskar speed exponentiellt tills hastigheten är nära 0, då sätts den till 0.
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
