#include "Sprite.h"
#include <iostream>

Sprite::Sprite(const std::string& imagePath, FLOAT xPos, FLOAT yPos, FLOAT xScale, FLOAT yScale)
	:pos(xPos, yPos), size(), imageSRV(nullptr), initialSize(), immutable(true)
{
	bool succeeded = false;
	HRESULT hr;

	succeeded = Load(imagePath);
	assert(succeeded);

	float gpuVariables[4] = {xPos, -yPos, size.x, size.y };
	hr = Backend::CreateConstCBuffer(&transformBuffer, gpuVariables, 16);
	assert(SUCCEEDED(hr));
	
}

Sprite::Sprite(const std::string& imagePath)
	:immutable(false)
{
	bool succeeded = false;
	HRESULT hr;

	succeeded = Load(imagePath);
	assert(succeeded);

	float gpuVariables[4] = { pos.x, -pos.y, size.x, size.y };
	hr = Backend::CreateDynamicCBuffer(&transformBuffer, gpuVariables, 16);
	assert(SUCCEEDED(hr));
}

Sprite* Sprite::Create(const std::string& imagePath, FLOAT xPos, FLOAT yPos, FLOAT xScale, FLOAT yScale)
{
	return new Sprite(imagePath, xPos, yPos, xScale, yScale);
}

Sprite* Sprite::Create(const std::string& imagePath)
{
	return new Sprite(imagePath);
}

void Sprite::Destroy(Sprite* sprite)
{
	sprite->transformBuffer->Release();
	sprite->imageSRV->Release();
	delete sprite;
}

void Sprite::SetPosition(FLOAT x, FLOAT y)
{
	if (immutable)
		return;

	pos.x = x;
	pos.y = y;
}

void Sprite::SetScale(FLOAT x, FLOAT y)
{
	if (immutable)
		return;

	size.x = initialSize.x * x;
	size.y = initialSize.y * y;
}

bool Sprite::Hovered()
{
	const DirectX::XMFLOAT2 mPos((float)Backend::GetMouse().GetXPos(), (float)Backend::GetMouse().GetYPos());
	return mPos.x > pos.x && mPos.x < pos.x + size.x
		&& mPos.y > pos.y && mPos.y < pos.y + size.y;
}

bool Sprite::Clicked()
{
	return Hovered() && Backend::GetMouse().LeftReleased();
}

void Sprite::Draw()
{
	ID3D11DeviceContext* devContext = Backend::GetDeviceContext();
	
	if (!immutable)
	{
		float gpuVariables[4] = { pos.x, -pos.y, size.x, size.y };
		Backend::UpdateBuffer(transformBuffer, gpuVariables, 16);
	}

	devContext->VSSetConstantBuffers(0, 1, &transformBuffer);
	devContext->PSSetShaderResources(0, 1, &imageSRV);
	devContext->Draw(4, 0);
}

bool Sprite::Load(const std::string& imagePath)
{
	HRESULT hr;

	int channels;
	unsigned char* imgData = stbi_load(imagePath.c_str(), &initialSize.x, &initialSize.y, &channels, 4);
	if (!imgData)
		return false;

	ID3D11Texture2D* resource;
	hr = Backend::CreateConstSRVTexture2D(&resource, imgData, (UINT)initialSize.x, (UINT)initialSize.y);
	stbi_image_free(imgData);

	if (FAILED(hr))
		return false;

	size = { (float)initialSize.x, (float)initialSize.y };

	hr = Backend::GetDevice()->CreateShaderResourceView(resource, nullptr, &imageSRV);
	resource->Release();

	return SUCCEEDED(hr);
}
