#include "Character.h"

Character::Character(Mesh* mesh)
	:Object(mesh, true), key(Backend::GetKeyboard()), velocity()
{
	Object::DropPtr(this);

	//Basic
	//maxSpeed = 0.1f;
	//minSpeed = -0.1f;
	//zSpeed = 0.0f;
	//xSpeed = 0.0f;
	//decreaseZSpeed = false;
	//decreaseXSpeed = false;
	//diagMove = false;

	//Movement

	//Slide
	canSlide = true;
	isSliding = false;
	slideSpeed = 0;


	//Jump
	jumpVelocity = 0;
	gravity;
	fallSpeed = -20;
	jumpHeight = 6;
	canJump = false;
	gravity = 300;

	maxSpeed = 15.0f;
	speed = 0.2f;
	counterForce = 0.01f;
	//Boost
	fuel = 10.0f;
	canBoost = false;

	joy.bBox = mesh->bBox;
}

Character::~Character()
{
}

void Character::Move()
{
	float dt = Backend::GetDeltaTime();

	maxSpeed = 10.0f;
	speed = 0.1f;
	counterForce = 0.01f;
	timer += dt;


	bool wsPressed = false;
	bool adPressed = false;

	slideSpeed = 1.2f;

	if (key.KeyDown(DIK_W))
	{
		velocity.y += speed;
		wsPressed = true;
	}
	else if (velocity.y > 0.f)
	{
		velocity.y -= counterForce;
		wsPressed = false;
	}
	if (key.KeyDown(DIK_S))
	{
		velocity.y -= speed;
		wsPressed = true;
	}
	else if (velocity.y < 0.f)
	{
		velocity.y += counterForce;
		wsPressed = false;
	}
	if (key.KeyDown(DIK_D))
	{
		velocity.x += speed;
		adPressed = true;
	}
	else if (velocity.x > 0.f)
	{
		velocity.x -= counterForce;
		adPressed = false;
	}
	if (key.KeyDown(DIK_A))
	{
		adPressed = true;
		velocity.x -= speed;
	}
	else if (velocity.x < 0.f)
	{
		velocity.x += counterForce;
		adPressed = false;
	}


	velocity.x *= 0.99f;
	velocity.y *= 0.99f;

	if (std::abs(velocity.x) < 0.001f)
	{
		velocity.x = 0;
	}
	if (std::abs(velocity.y) < 0.001f)
	{
		velocity.y = 0;
	}
	if (std::abs(velocity.x) > 0.0f && std::abs(velocity.y) > 0.0f && wsPressed == true && adPressed == true)
	{
		maxSpeed = 5.0f;
	}
	else
	{
		maxSpeed = 10.0f;
	}


	if (key.KeyDown(DIK_LSHIFT) && fuel > 0 && isSliding == false && timer > 2.5f)
	{
		isSliding = true;
		canSlide = false;
		timer = 0;
	}
	if (timer < 0.5f && isSliding == true)
	{
		slideSpeed += 1.7f;
	}
	else
	{
		isSliding = false;
	}



	if (std::abs(velocity.x) > maxSpeed)
		velocity.x *= 0.99f;

	if (std::abs(velocity.y) > maxSpeed)
		velocity.y *= 0.99f;


	Translate(velocity.x * slideSpeed * dt, 0.0f, velocity.y * slideSpeed * dt);

}

void Character::Jump()
{
	float dt = Backend::GetDeltaTime();


	jumpVelocity -= gravity * dt;
	if (isOnGround)
	{
		jumpVelocity = 0;
		canJump = true;
		canBoost = false;
	}
	else
	{
		canJump = false;
	}


	//Jump
	if (key.KeyDown(DIK_SPACE) && canJump)
	{
		canJump = false;
		jumpVelocity += std::sqrtf(2.0f * gravity * jumpHeight);
		isOnGround = false;
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
	else if (jumpVelocity < fallSpeed)
	{
		jumpVelocity = fallSpeed;
	}

	this->Translate(0, jumpVelocity * dt, 0);
}

void Character::Respawn()
{
	//Checks if the player has fallen too far off the map and sets the position back to start
	if (GetPosition().y < -10.0f)
		SetPosition(0.0f, 0.0f, 0.0f);
}



bool Character::SetCanJump(bool canJump)
{
	isOnGround = canJump;
	return isOnGround;
}









