#include "Character.h"

Character::Character(Mesh* mesh)
	:Object(mesh, false), key(Backend::GetKeyboard()), velocity()
	, head(mesh/*->GetChild()*/, false)
	, leftArm(mesh/*->GetChild()*/, false)
	, rightArm(mesh/*->GetChild()*/, false)
{
	Object::DropPtr(this);
	Object::DropPtr(&head);
	Object::DropPtr(&leftArm);
	Object::DropPtr(&rightArm);

	head.SetPosition(0.f, 1.f, 0.f);
	rightArm.SetPosition(1.f, 0.f, 0.f);
	leftArm.SetPosition(-1.f, 0.f, 0.f);

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
	head.Rotate(0.f, 1.f * dt, 0.f);
	leftArm.Rotate(1.f * dt, 0.f, 0.f);
	rightArm.Rotate(-1.f * dt, 0.f, 0.f);

	bool wsPressed = false;
	bool adPressed = false;

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

	/*std::cout << velocity.x << "===";
	std::cout << velocity.y<< std::endl;*/
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
		maxSpeed = 10.0f;

	if (std::abs(velocity.x) > maxSpeed)
		velocity.x *= 0.99f;

	if (std::abs(velocity.y) > maxSpeed)
		velocity.y *= 0.99f;


	Translate(velocity.x * dt, 0.0f, velocity.y * dt);

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
