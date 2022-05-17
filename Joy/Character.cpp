#include "Character.h"

Character::Character(Mesh* mesh)
	:Object(mesh, false), key(Backend::GetKeyboard()), velocity()
	, head(mesh/*->GetChild()*/, false)
	, leftArm(mesh/*->GetChild()*/, false)
	, rightArm(mesh/*->GetChild()*/, false)
	, isRotating(false)
	, rotateBack(0.f)
{
	Object::DropPtr(this);
	Object::DropPtr(&head);
	Object::DropPtr(&leftArm);
	Object::DropPtr(&rightArm);

	head.SetPosition(0.f, 1.f, 0.f);
	rightArm.SetPosition(1.f, 0.f, 0.f);
	leftArm.SetPosition(-1.f, 0.f, 0.f);


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
	head.Rotate(0.f, 20.f * dt, 0.f);
	leftArm.Rotate(50.f * dt, 0.f, 0.f);
	rightArm.Rotate(-50.f * dt, 0.f, 0.f);

	maxSpeed = 10.0f;
	speed = 0.1f;
	counterForce = 0.01f;
	timer += dt;
	rotTimer += dt;


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

	//SLIDE
	if (key.KeyDown(DIK_LSHIFT) && fuel > 0 && isSliding == false && timer > 2.5f)
	{
		isSliding = true;
		canSlide = false;
		timer = 0;
	}
	if ((timer < 0.5f && isSliding == true) || (key.KeyReleased(DIK_LSHIFT) && isSliding == true))
	{
		slideSpeed += 1.7f;
	}
	else
	{
		isSliding = false;
	}
	//
	//ROTATE WHILE SLIDING
	rotateVal = 6.f;
	if (isSliding && rotTimer > 2.5f)
	{
		rotTimer = 0.f;
	}
	if (key.KeyDown(DIK_LSHIFT) && GetRotation().x < 1.3 && key.KeyDown(DIK_W) && rotTimer < 0.25f)
	{
		rotateVal *= dt;
		Rotate(rotateVal, 0.f, 0.f);
		isRotating = true;
		rotateBack += rotateVal;
	}
	if (isSliding && (key.KeyDown(DIK_W) && key.KeyDown(DIK_A)) && rotTimer < 0.25f)
	{
		SetRotation(GetRotation().x, -0.5f, GetRotation().z);
	}
	if (isSliding && (key.KeyDown(DIK_W) && key.KeyDown(DIK_D)) && rotTimer < 0.25f)
	{
		SetRotation(GetRotation().x, 0.5f, GetRotation().z);
	}
	else if (isSliding && GetRotation().x < 1.3 && key.KeyDown(DIK_S) && rotTimer < 0.25f)
	{
		rotateVal *= dt;
		Rotate(-rotateVal, 0.f, 0.f);
		isRotating = true;
		rotateBack += rotateVal;
	}
	if (isSliding && (key.KeyDown(DIK_S) && key.KeyDown(DIK_A)) && rotTimer < 0.25f)
	{
		SetRotation(GetRotation().x, 0.5f, GetRotation().z);
	}
	if (isSliding && (key.KeyDown(DIK_S) && key.KeyDown(DIK_D)) && rotTimer < 0.25f)
	{
		SetRotation(GetRotation().x, -0.5f, GetRotation().z);
	}
	else if (isSliding && GetRotation().z < 1.3 && key.KeyDown(DIK_A) && rotTimer < 0.25f)
	{
		rotateVal *= dt;
		Rotate(0.f, 0.f, rotateVal);
		isRotating = true;
		rotateBack += rotateVal;
	}
	else if (isSliding && GetRotation().z < 1.3 && key.KeyDown(DIK_D) && rotTimer < 0.25f)
	{
		rotateVal *= dt;
		Rotate(0.f, 0.f, -rotateVal);
		isRotating = true;
		rotateBack += rotateVal;
	}
	//
	// ROTATE BACK
	if (isRotating && (key.KeyReleased(DIK_W) || key.KeyReleased(DIK_S) || key.KeyReleased(DIK_A) || key.KeyReleased(DIK_D)))
	{
		SetRotation(0.f, 0.f, 0.f);
		isRotating = false;
	}
	//

	if (std::abs(velocity.x) > maxSpeed)
		velocity.x *= 0.99f;

	if (std::abs(velocity.y) > maxSpeed)
		velocity.y *= 0.99f;

	if (isSliding)
	{
		fuel -= 1 * dt;
	}



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

	//Boost
	if (canBoost && key.KeyDown(DIK_SPACE))
	{
		jumpVelocity += 325 * dt;
		fuel -= 1 * dt;  // how much fuel used
	}
	else if (jumpVelocity < fallSpeed)
	{
		jumpVelocity = fallSpeed;
	}

	fuel += 0.5f * dt; // how much increases

	if (fuel > 10.f) // max fuel sett
	{
		fuel = 10.f;
	}

	this->Translate(0, jumpVelocity * dt, 0);

	//std::cout << fuel << std::endl;
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

void Character::Draw()
{
	ID3D11DeviceContext* dc = Backend::GetDeviceContext();

	Mesh* mesh = GetMesh();

	dc->IASetVertexBuffers(0, 1, &mesh->vertexBuffer, &Mesh::Stirde, &Mesh::Offset);
	dc->IASetIndexBuffer(mesh->indexBuffer, DXGI_FORMAT_R32_UINT, Mesh::Offset);
	//mesh->Bind();

	dc->VSSetConstantBuffers(0, 1, GetTransformBuffer());
	dc->PSSetShaderResources(0, 1, &mesh->diffuseTextureSRV);

	if (mesh->indexBuffer)
		dc->DrawIndexed(mesh->indexCount, 0, 0);
	else
		dc->Draw(mesh->indexCount, 0);

	DrawChildren();
}

void Character::DrawChildren()
{
	const DirectX::XMMATRIX& matrix = GetWorldMatrixXM();

	head.ApplyParentTransform(matrix);
	leftArm.ApplyParentTransform(matrix);
	rightArm.ApplyParentTransform(matrix);

	head.DrawGeometry();
	leftArm.DrawGeometry();
	rightArm.DrawGeometry();
}
