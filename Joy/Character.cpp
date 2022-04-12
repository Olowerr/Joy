#include "Character.h"

Character::Character(Mesh* mesh)
	:Object(mesh), key(Backend::GetKeyboard())
{
	maxSpeed = 10.0f;
	maxDiagSpeed = 5.0f;
	minSpeed = 0.0f;
	speed = 0.00005f;
	Character::decreaseSpeed = true;
	Character::increaseSpeed = false;
}

void Character::move()
{
	if (key.KeyDown(DIK_W))
	{
		speed = 0.00005f;
		increaseSpeed = true;
		decreaseSpeed = false;
		while (speed <= maxSpeed)
		{
			(speed *= 1.05)*Backend::GetDeltaTime();
			Translate(0.0f, 0.0f,speed );
		}
		
		if (key.KeyDown(DIK_A))
		{
			if (speed > maxDiagSpeed)
			{
				speed = maxDiagSpeed/2;
			}
			while (speed <= maxDiagSpeed)
			{
				speed *= 1.05;
				Translate(-speed / 2, 0.0f, speed / 2);
			}
		}
		if (key.KeyDown(DIK_D))
		{
			if (speed > maxDiagSpeed)
			{
				speed = maxDiagSpeed/2;
			}
			while (speed <= maxDiagSpeed)
			{
				speed *= 1.05f;
				Translate(speed / 2, 0.0f, speed / 2);
			}
		}
	}
	else if (key.KeyDown(DIK_A))
	{
		speed = 0.00005f;
		increaseSpeed = true;
		decreaseSpeed = false;
		while (speed <= maxSpeed)
		{
			(speed *= 1.05) * Backend::GetDeltaTime();
			Translate(-speed, 0.0f, 0.0f);
		}
		if (key.KeyDown(DIK_W))
		{
			if (speed > maxDiagSpeed)
			{
				speed = maxDiagSpeed / 2;
			}
			while (speed <= maxDiagSpeed)
			{
				speed *= 1.05;
				Translate(-speed / 2, 0.0f, speed / 2);
			}
		}
		if (key.KeyDown(DIK_S))
		{
			if (speed > maxDiagSpeed)
			{
				speed = maxDiagSpeed / 2;
			}
			while (speed <= maxDiagSpeed)
			{
				speed *= 1.05;
				Translate(-speed / 2, 0.0f, -speed / 2);
			}
		}
	}
	else if (key.KeyDown(DIK_S))
	{
		speed = 0.00005f;
		increaseSpeed = true;
		decreaseSpeed = false;
		while (speed <= maxSpeed)
		{
			(speed *= 1.05) * Backend::GetDeltaTime();
			Translate(0.0f, 0.0f, -speed);
		}
		if (key.KeyDown(DIK_A))
		{
			if (speed > maxDiagSpeed)
			{
				speed = maxDiagSpeed / 2;
			}
			while (speed <= maxDiagSpeed)
			{
				speed *= 1.05;
				Translate(-speed / 2, 0.0f, -speed / 2);
			}
		}
		if (key.KeyDown(DIK_D))
		{
			if (speed > maxDiagSpeed)
			{
				speed = maxDiagSpeed / 2;
			}
			while (speed <= maxDiagSpeed)
			{
				speed *= 1.05;
				Translate(speed / 2, 0.0f, -speed / 2);
			}
		}
	}
	else if (key.KeyDown(DIK_D))
	{
		speed = 0.00005f;
		increaseSpeed = true;
		decreaseSpeed = false;
		while (speed <= maxSpeed)
		{
			(speed *= 1.05) * Backend::GetDeltaTime();
			Translate(speed, 0.0f, 0.0f);
		}
		if (key.KeyDown(DIK_S))
		{
			if (speed > maxDiagSpeed)
			{
				speed = maxDiagSpeed / 2;
			}
			while (speed <= maxDiagSpeed)
			{
				speed *= 1.05;
				Translate(speed / 2, 0.0f, -speed / 2);
			}
		}
		if (key.KeyDown(DIK_W))
		{
			if (speed > maxDiagSpeed)
			{
				speed = maxDiagSpeed / 2;
			}
			while (speed <= maxDiagSpeed)
			{
				speed *= 1.05;
				Translate(speed / 2, 0.0f, speed / 2);
			}
		}
	}
	else
	{
		increaseSpeed = false;
		decreaseSpeed = true;
		if (speed > 0.1f&&decreaseSpeed==true)
		{
			speed *= 0.5f;
		}
		else
		{
			speed = 0;
		}
	}
}
