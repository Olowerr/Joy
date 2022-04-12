#include "Character.h"

Character::Character(Mesh* mesh)
	:Object(mesh), key(Backend::GetKeyboard())
{
	maxSpeed = 10.0f;
	maxDiagSpeed = 5.0f;
	minSpeed = 0.0f;
	speed = 1.0f;
}

void Character::move()
{
	if (key.KeyDown(DIK_W))
	{
		increaseSpeed = true;
		decreaseSpeed = false;
		while (speed <= maxSpeed)
		{
			speed *= 1.05;
			Translate(0.0f, 0.0f,speed*1.0f );
		}
		
		if (key.KeyDown(DIK_A))
		{
			while (speed <= maxDiagSpeed)
			{
				speed *= 1.05;
				Translate((speed*-1.0f) / 2, 0.0f, speed / 2);
			}
		}
		if (key.KeyDown(DIK_D))
		{

		}
	}
	else if (key.KeyDown(DIK_A))
	{
		increaseSpeed = true;
		decreaseSpeed = false;
		if (key.KeyDown(DIK_W))
		{

		}
		if (key.KeyDown(DIK_S))
		{

		}
	}
	else if (key.KeyDown(DIK_S))
	{
		increaseSpeed = true;
		decreaseSpeed = false;
		if (key.KeyDown(DIK_A))
		{

		}
		if (key.KeyDown(DIK_D))
		{

		}
	}
	else if (key.KeyDown(DIK_D))
	{
		increaseSpeed = true;
		decreaseSpeed = false;
		if (key.KeyDown(DIK_S))
		{

		}
		if (key.KeyDown(DIK_W))
		{

		}
	}
	else
	{
		increaseSpeed = false;
		decreaseSpeed = true;
		if (speed > 0.1f)
		{
			speed *= 0.5f;
		}
		else
		{
			speed = 0;
		}
	}
}
