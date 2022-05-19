#include "Character.h"

Character::Character(Mesh* mesh)
	:Object(mesh, false), key(Backend::GetKeyboard()), velocity()
	, head(mesh->children[1], false)
	, arms(mesh->children[0], false)
	, isRotating(false)
	, rotateBack(0.f)
	, charRotation(5.f)
{
	Object::DropPtr(this);
	Object::DropPtr(&head);
	Object::DropPtr(&arms);

	arms.SetPosition(0.f, 1.2f, 0.f);

	LoadGlowMap();

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
	bBoxExtents = GetBoundingBox(0).Extents;
}

Character::~Character()
{
}

void Character::Shutdown()
{
	Object::Shutdown();
	head.Shutdown();
	arms.Shutdown();

	glowMapSRV->Release();
}

void Character::Move()
{
	float dt = Backend::GetDeltaTime();

	arms.Rotate(0.1f, 0.f, 0.f);

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
	if (key.KeyDown(DIK_W) && !key.KeyDown(DIK_D) && !key.KeyDown(DIK_A))
	{
		if (GetRotation().y > -0.01 && GetRotation().y < 0.01)
		{
			SetRotation(GetRotation().x, 0.f, GetRotation().z);
		}
		else if (GetRotation().y < 0.f)
		{
			Rotate(0.f, charRotation * dt, 0.f);
		}
		else if (GetRotation().y > 0.f)
		{
			Rotate(0.f, -charRotation * dt, 0.f);
		}
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
	if (key.KeyDown(DIK_S) && !key.KeyDown(DIK_D) && !key.KeyDown(DIK_A))
	{
		if (GetRotation().y > 3.139 && GetRotation().y < 3.141f)
		{
			SetRotation(GetRotation().x, 3.14f, GetRotation().z);
		}
		else if (GetRotation().y < 3.14f && GetRotation().y >= 0.f)
		{
			Rotate(0.f, charRotation * dt, 0.f);
		}
		else if (GetRotation().y > -3.14f && GetRotation().y < 0.f)
		{
			Rotate(0.f, -charRotation * dt, 0.f);
		}
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
	if (key.KeyDown(DIK_D) && key.KeyDown(DIK_W))
	{
		if (GetRotation().y > 0.784 && GetRotation().y < 0.786f)
		{
			SetRotation(GetRotation().x, 0.785f, GetRotation().z);
		}
		else if (GetRotation().y < 0.785)
		{
			Rotate(0.f, charRotation * dt, 0.f);
		}
		else if (GetRotation().y > 0.785)
		{
			Rotate(0.f, -charRotation * dt, 0.f);
		}
	}
	if (key.KeyDown(DIK_D) && key.KeyDown(DIK_S))
	{
		if (GetRotation().y > 2.34 && GetRotation().y < 2.36f)
		{
			SetRotation(GetRotation().x, 2.35f, GetRotation().z);
		}
		else if (GetRotation().y < 2.35f)
		{
			Rotate(0.f, charRotation * dt, 0.f);
		}
		else if (GetRotation().y > 2.35f)
		{
			Rotate(0.f, -charRotation * dt, 0.f);
		}
	}
	if (key.KeyDown(DIK_D) && !key.KeyDown(DIK_W))
	{
		if (GetRotation().y < 1.57f)
		{
			Rotate(0.f, charRotation * dt, 0.f);
		}
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
	if (key.KeyDown(DIK_A) && key.KeyDown(DIK_W))
	{
		if (GetRotation().y < -0.780 && GetRotation().y > -0.790f)
		{
			SetRotation(GetRotation().x, -0.785f, GetRotation().z);
		}
		if (GetRotation().y > -0.785f)
		{
			Rotate(0.f, -charRotation * dt, 0.f);
		}
		else if (GetRotation().y < -0.785f)
		{
			Rotate(0.f, charRotation * dt, 0.f);
		}
	}
	if (key.KeyDown(DIK_A) && key.KeyDown(DIK_S))
	{
		if (GetRotation().y < -2.34 && GetRotation().y > 2.36f)
		{
			SetRotation(GetRotation().x, -2.35f, GetRotation().z);
		}
		if (GetRotation().y > -2.35f)
		{
			Rotate(0.f, -charRotation * dt, 0.f);
		}
		else if (GetRotation().y < -2.35f)
		{
			Rotate(0.f, charRotation * dt, 0.f);
		}
	}
	if (key.KeyDown(DIK_A) && !key.KeyDown(DIK_W))
	{
		if (GetRotation().y > -1.57f)
		{
			Rotate(0.f, -charRotation * dt, 0.f);
		}
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
	if ((timer < 0.8f && isSliding == true) || (key.KeyReleased(DIK_LSHIFT) && isSliding == true))
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
		SetBBox(0, { GetBoundingBox(0).Center.x, GetPosition().y - 0.1f, GetBoundingBox(0).Center.z + 0.023f }, { 0.5f, 0.5f, 1.1f });
	}
	else if (isSliding && GetRotation().x < 1.3 && key.KeyDown(DIK_S) && rotTimer < 0.25f)
	{
		rotateVal *= dt;
		Rotate(rotateVal, 0.f, 0.f);
		isRotating = true;
		rotateBack += rotateVal;
		SetBBox(0, { GetBoundingBox(0).Center.x, GetPosition().y - 0.1f, GetBoundingBox(0).Center.z - 0.023f }, { 0.5f, 0.5f, 1.1f });
	}
	else if (isSliding && GetRotation().x < 1.3 && key.KeyDown(DIK_A) && rotTimer < 0.25f)
	{
		rotateVal *= dt;
		Rotate(rotateVal, 0.f, 0.f);
		isRotating = true;
		rotateBack += rotateVal;
		SetBBox(0, { GetBoundingBox(0).Center.x - 0.023f, GetPosition().y + 0.1f, GetBoundingBox(0).Center.z }, { 1.1f, 0.5f, 0.5f });
	}
	else if (isSliding && GetRotation().x < 1.3 && key.KeyDown(DIK_D) && rotTimer < 0.25f)
	{
		rotateVal *= dt;
		Rotate(rotateVal, 0.f, 0.f);
		isRotating = true;
		rotateBack += rotateVal;
		SetBBox(0, { GetBoundingBox(0).Center.x + 0.023f, GetPosition().y - 0.1f, GetBoundingBox(0).Center.z }, { 1.1f, 0.5f, 0.5f });
	}
	//
	// ROTATE BACK
	if (isRotating && timer > 1.f || isRotating && (key.KeyReleased(DIK_W) || key.KeyReleased(DIK_S) || key.KeyReleased(DIK_A) || key.KeyReleased(DIK_D)))
	{
		SetRotation(0.f, 0.f, 0.f);
		isRotating = false;
		SetBBox(0, { GetPosition().x, GetPosition().y + 1.f,GetPosition().z }, bBoxExtents);
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
	dc->PSSetShaderResources(0, 1, &mesh->diffuseTextureSRV);

	dc->IASetVertexBuffers(0, 1, &mesh->vertexBuffer, &Mesh::Stirde, &Mesh::Offset);
	dc->IASetIndexBuffer(mesh->indexBuffer, DXGI_FORMAT_R32_UINT, Mesh::Offset);
	//mesh->Bind();


	dc->VSSetConstantBuffers(0, 1, GetTransformBuffer());

	dc->PSSetShaderResources(0, 1, &mesh->diffuseTextureSRV);
	dc->PSSetShaderResources(1, 1, &glowMapSRV);


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
	arms.ApplyParentTransform(matrix);
	//rightArm.ApplyParentTransform(matrix);

	head.DrawGeometry();
	arms.DrawGeometry();
	//rightArm.DrawGeometry();
}

void Character::LoadGlowMap()
{
	HRESULT hr;

	int width, height, channels;

	unsigned char* imageData = stbi_load("../Resources/JOYFiles/JoyGlowMap.png", &width, &height, &channels, 1);
	if (!imageData)
		return;

	D3D11_TEXTURE2D_DESC desc{};
	desc.Format = DXGI_FORMAT_R8_UNORM;
	desc.ArraySize = 1;
	desc.MipLevels = 1;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.Height = height;
	desc.Width = width;
	desc.MiscFlags = 0;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_IMMUTABLE;

	D3D11_SUBRESOURCE_DATA inData{};
	inData.pSysMem = imageData;
	inData.SysMemPitch = width * 1;
	inData.SysMemSlicePitch = 0;

	ID3D11Texture2D* resource;
	hr = Backend::GetDevice()->CreateTexture2D(&desc, &inData, &resource);

	stbi_image_free(imageData);
	if (FAILED(hr))
		return;

	Backend::GetDevice()->CreateShaderResourceView(resource, nullptr, &glowMapSRV);
	resource->Release();
}
