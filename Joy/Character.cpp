#include "Character.h"

Character::Character(Mesh* mesh)
	:Object(mesh), key(Backend::GetKeyboard())
{
	maxSpeed = 10.0f;
	minSpeed = 0.0f;
	speed = 0.0f;
	speed2 = 0.0f;
	decreaseSpeed = false;
	decreaseSpeed2 = false;
	diagMove = false;
}

void Character::move()
{
	//Känner av knapptryck och rör karaktären i den rikting man trycker
	if (key.KeyDown(DIK_W)&& speed >= -0.001f)
	{
		decreaseSpeed = false;
		speed += 0.04f * Backend::GetDeltaTime();
	}
	if (key.KeyReleased(DIK_W))
	{
		decreaseSpeed = true;
	}


	if (key.KeyDown(DIK_A)&& speed2 <= 0.001f)
	{
		decreaseSpeed2 = false;
		speed2 -= 0.04f * Backend::GetDeltaTime();
	}
	if (key.KeyReleased(DIK_A))
	{
		decreaseSpeed2 = true;
	}


	if (key.KeyDown(DIK_S)&& speed <= 0.001f)
	{
		decreaseSpeed = false;
		speed -= 0.04f * Backend::GetDeltaTime();
	}
	if (key.KeyReleased(DIK_S))
	{
		decreaseSpeed = true;
	}


	if (key.KeyDown(DIK_D)&& speed2 >= -0.001f)
	{
		decreaseSpeed2 = false;
		speed2 += 0.04f * Backend::GetDeltaTime();
	}
	if (key.KeyReleased(DIK_D))
	{
		decreaseSpeed2 = true;
	}


	//Flyttar karaktären och ser till att diagonal rörelse inte är snabbare än rakt fram/bak
	if(diagMove == false)
		Translate(speed2, 0.0f, speed);
	if (speed != 0.0f && speed2 != 0.0f)
	{
		diagMove = true;
		Translate(speed2 / 2, 0.0f, speed / 2);
	}
	else
		diagMove = false;
		

	//Minskar speed exponentiellt tills hastigheten är nära 0, då sätts den till 0.
	if (decreaseSpeed == true)
	{
		speed *= 0.98f;
		if (speed < 0.0001f&&speed> 0.0f)
		{
			decreaseSpeed = false;
			speed = 0.0f;
		}
		else if (speed > -0.0001f && speed < 0.0f)
		{
			decreaseSpeed = false;
			speed = 0.0f;
		}
	}
	if (decreaseSpeed2 == true)
	{
		speed2 *= 0.98f;
		if (speed2 < 0.0001f && speed2> 0.0f)
		{
			decreaseSpeed2 = false;
			speed2 = 0.0f;
		}
		else if (speed2 > -0.0001f && speed2 < 0.0f)
		{
			decreaseSpeed2 = false;
			speed2 = 0.0f;
		}
	}




	/*if (key.KeyDown(DIK_W))
	{
		speed = 1.0f;
		increaseSpeed = true;
		decreaseSpeed = false;
		if (speed <= maxSpeed)
		{
			speed *= 1.05*Backend::GetDeltaTime();
			Translate(0.0f, 0.0f, speed);
		}
		
		if (key.KeyDown(DIK_A))
		{
			if (speed > maxDiagSpeed)
			{
				speed = maxDiagSpeed/2;
			}
			if (speed <= maxDiagSpeed)
			{
				speed *= 1.05f * Backend::GetDeltaTime();;
				Translate(-speed / 2, 0.0f, speed / 2);
			}
		}
		if (key.KeyDown(DIK_D))
		{
			if (speed > maxDiagSpeed)
			{
				speed = maxDiagSpeed/2;
			}
			if (speed <= maxDiagSpeed)
			{
				speed *= 1.05f * Backend::GetDeltaTime();;
				Translate(speed / 2, 0.0f, speed / 2);
			}
		}
	}
	else if (key.KeyDown(DIK_A))
	{
		speed = 1.0f;
		increaseSpeed = true;
		decreaseSpeed = false;
		if (speed <= maxSpeed)
		{
			speed *= 1.05f * Backend::GetDeltaTime();;
			Translate(-speed, 0.0f, 0.0f);
		}
		if (key.KeyDown(DIK_W))
		{
			if (speed > maxDiagSpeed)
			{
				speed = maxDiagSpeed / 2;
			}
			if (speed <= maxDiagSpeed)
			{
				speed *= 1.05f * Backend::GetDeltaTime();;
				Translate(-speed / 2, 0.0f, speed / 2);
			}
		}
		if (key.KeyDown(DIK_S))
		{
			if (speed > maxDiagSpeed)
			{
				speed = maxDiagSpeed / 2;
			}
			if (speed <= maxDiagSpeed)
			{
				speed *= 1.05f * Backend::GetDeltaTime();;
				Translate(-speed / 2, 0.0f, -speed / 2);
			}
		}
	}
	else if (key.KeyDown(DIK_S))
	{
		speed = 1.0f;
		increaseSpeed = true;
		decreaseSpeed = false;
		if (speed <= maxSpeed)
		{
			speed *= 1.05f * Backend::GetDeltaTime();;
			Translate(0.0f, 0.0f, -speed);
		}
		if (key.KeyDown(DIK_A))
		{
			if (speed > maxDiagSpeed)
			{
				speed = maxDiagSpeed / 2;
			}
			if (speed <= maxDiagSpeed)
			{
				speed *= 1.05f * Backend::GetDeltaTime();;
				Translate(-speed / 2, 0.0f, -speed / 2);
			}
		}
		if (key.KeyDown(DIK_D))
		{
			if (speed > maxDiagSpeed)
			{
				speed = maxDiagSpeed / 2;
			}
			if (speed <= maxDiagSpeed)
			{
				speed *= 1.05f * Backend::GetDeltaTime();;
				Translate(speed / 2, 0.0f, -speed / 2);
			}
		}
	}
	else if (key.KeyDown(DIK_D))
	{
		speed = 1.0f;
		increaseSpeed = true;
		decreaseSpeed = false;
		if (speed <= maxSpeed)
		{
			speed *= 1.05f * Backend::GetDeltaTime();;
			Translate(speed, 0.0f, 0.0f);
		}
		if (key.KeyDown(DIK_S))
		{
			if (speed > maxDiagSpeed)
			{
				speed = maxDiagSpeed / 2;
			}
			if (speed <= maxDiagSpeed)
			{
				speed *= 1.05f * Backend::GetDeltaTime();;
				Translate(speed / 2, 0.0f, -speed / 2);
			}
		}
		if (key.KeyDown(DIK_W))
		{
			if (speed > maxDiagSpeed)
			{
				speed = maxDiagSpeed / 2;
			}
			if (speed <= maxDiagSpeed)
			{
				speed *= 1.05f * Backend::GetDeltaTime();;
				Translate(speed / 2, 0.0f, speed / 2);
			}
		}
	}
	*/
	/*else
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
	*/
}
