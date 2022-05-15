#include "Object.h"
#include <iostream>

Object::Object(Mesh* mesh, bool levelObject)
	:mesh(mesh), lightMap(nullptr), IsLevelObject(levelObject), isInstanced(false)
{
	bBox.emplace_back(mesh->bBox);

	if (levelObject)
		levelObjects.emplace_back(this);
	else
		enviormentObjects.emplace_back(this);
}

Object::Object(Mesh* mesh, bool levelObject, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, FLOAT scale)
	:Transform(pos, rot, scale), mesh(mesh), lightMap(nullptr), IsLevelObject(levelObject), isInstanced(false)
{
	bBox.emplace_back(mesh->bBox);
	SetBBox(0, pos, scale);

	if (levelObject)
		levelObjects.emplace_back(this);
	else
		enviormentObjects.emplace_back(this);
}

void Object::Shutdown()
{
	DropPtr(this);

	Transform::Shutdown();

	if (lightMap)
		lightMap->Release();
}

Object::~Object()
{
	DropPtr(this);
}

void Object::CheckBB()
{
	bBox.at(0) = mesh->bBox;
}

void Object::Draw()
{
	ID3D11DeviceContext* dc = Backend::GetDeviceContext();

	dc->IASetVertexBuffers(0, 1, &mesh->vertexBuffer, &Mesh::Stirde, &Mesh::Offset);
	dc->IASetIndexBuffer(mesh->indexBuffer, DXGI_FORMAT_R32_UINT, Mesh::Offset);
	//mesh->Bind();

	dc->VSSetConstantBuffers(0, 1, GetTransformBuffer());
	dc->PSSetShaderResources(0, 1, &mesh->diffuseTextureSRV);
	dc->PSSetShaderResources(2, 1, &lightMap);

	if (mesh->indexBuffer)
		dc->DrawIndexed(mesh->indexCount, 0, 0);
	else
		dc->Draw(mesh->indexCount, 0);
}

void Object::DrawGeometry()
{
	ID3D11DeviceContext* dc = Backend::GetDeviceContext();

	dc->IASetVertexBuffers(0, 1, &mesh->vertexBuffer, &Mesh::Stirde, &Mesh::Offset);
	dc->IASetIndexBuffer(mesh->indexBuffer, DXGI_FORMAT_R32_UINT, Mesh::Offset);
	//mesh->BindGeometry();

	dc->VSSetConstantBuffers(0, 1, GetTransformBuffer());

	if (mesh->indexBuffer)
		dc->DrawIndexed(mesh->indexCount, 0, 0);
	else
		dc->Draw(mesh->indexCount, 0);
}

void Object::Translate(const DirectX::XMVECTOR& movement)
{
	Transform::Translate(movement);
	for (int i = 0; i < (int)bBox.size(); i++)
	{
		bBox.at(i).Center.x += movement.m128_f32[0];
		bBox.at(i).Center.y += movement.m128_f32[1];
		bBox.at(i).Center.z += movement.m128_f32[2];
	}
}

void Object::Translate(FLOAT X, FLOAT Y, FLOAT Z)
{
	Transform::Translate(X, Y, Z);
	for (int i = 0; i < (int)bBox.size(); i++)
	{
		bBox.at(i).Center.x += X;
		bBox.at(i).Center.y += Y;
		bBox.at(i).Center.z += Z;
	}
}

void Object::SetPosition(const DirectX::XMVECTOR& position)
{
	using namespace DirectX;
	const XMFLOAT3 prevPos = GetPosition();

	Transform::SetPosition(position);

	XMFLOAT3 delta;
	for (int i = 0; i < (int)bBox.size(); i++)
	{
		delta.x = bBox.at(i).Center.x - prevPos.x;
		delta.y = bBox.at(i).Center.y - prevPos.y;
		delta.z = bBox.at(i).Center.z - prevPos.z;

		bBox.at(i).Center.x = delta.x + position.m128_f32[0];
		bBox.at(i).Center.y = delta.y + position.m128_f32[1];
		bBox.at(i).Center.z = delta.z + position.m128_f32[2];
	}
}

void Object::SetPosition(FLOAT X, FLOAT Y, FLOAT Z)
{
	using namespace DirectX;
	const XMFLOAT3 prevPos = GetPosition();

	Transform::SetPosition(X, Y, Z);

	XMFLOAT3 delta;
	for (int i = 0; i < (int)bBox.size(); i++)
	{
		delta.x = bBox.at(i).Center.x - prevPos.x;
		delta.y = bBox.at(i).Center.y - prevPos.y;
		delta.z = bBox.at(i).Center.z - prevPos.z;

		bBox.at(i).Center.x = delta.x + X;
		bBox.at(i).Center.y = delta.y + Y;
		bBox.at(i).Center.z = delta.z + Z;
	}
}

void Object::AddBBox()
{
	bBox.emplace_back(mesh->bBox);
}

void Object::AddBBox(DirectX::XMFLOAT3 center, DirectX::XMFLOAT3 extents)
{
	bBox.emplace_back(center, extents);
}

void Object::SetBBox(int bBoxIndex, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 extents)
{
	if (bBoxIndex < 0 || bBoxIndex >= bBox.size())
		return;

	bBox.at(bBoxIndex).Center = pos;
	bBox.at(bBoxIndex).Extents = extents;
}

void Object::SetBBox(int bBoxIndex, DirectX::XMFLOAT3 pos, FLOAT scale)
{
	if (bBoxIndex < 0 || bBoxIndex >= bBox.size())
		return;

	bBox.at(bBoxIndex).Center = pos;
	bBox.at(bBoxIndex).Extents.x *= scale;
	bBox.at(bBoxIndex).Extents.y *= scale;
	bBox.at(bBoxIndex).Extents.z *= scale;
}

void Object::RemoveBBox(int bBoxIndex)
{
	if (bBoxIndex < 0 || bBoxIndex >= bBox.size())
		return;

	bBox.erase(bBox.begin() + bBoxIndex);
}

void Object::Scale(FLOAT amount)
{
	Transform::Scale(amount);
	for (int i = 0; i < (int)bBox.size(); i++)
	{
		bBox.at(i).Extents.x *= amount;
		bBox.at(i).Extents.y *= amount;
		bBox.at(i).Extents.z *= amount;
	}
}

void Object::SetScale(FLOAT Scale)
{
	Transform::SetScale(Scale);
	/*bBox.Extents.x = mesh->bBox.Extents.x * Scale;
	bBox.Extents.y = mesh->bBox.Extents.y * Scale;
	bBox.Extents.z = mesh->bBox.Extents.z * Scale;*/

	using namespace DirectX;

	XMVECTOR objectPos = DirectX::XMLoadFloat3(&GetPosition());
	for (int i = 0; i < (int)bBox.size(); i++)
	{
		XMVECTOR scaledDelta = (DirectX::XMLoadFloat3(&mesh->bBox.Center) - objectPos) * Scale;

		XMStoreFloat3(&bBox[i].Center, objectPos + scaledDelta);

		bBox.at(i).Extents.x = mesh->bBox.Extents.x * Scale;
		bBox.at(i).Extents.y = mesh->bBox.Extents.y * Scale;
		bBox.at(i).Extents.z = mesh->bBox.Extents.z * Scale;
	}
}

const DirectX::BoundingBox& Object::GetBoundingBox(int index) const
{
	if (index < 0 || index >= bBox.size())
		return mesh->bBox; // need to return something, can't be local variable

	return bBox.at(index);
}

const int Object::GetNumBboxes()
{
	return bBox.size();
}

Mesh* Object::GetMesh()
{
	return mesh;
}

ID3D11ShaderResourceView** Object::GetLightMapSRV()
{
	return &lightMap;
}

void Object::SetIsInstanced(bool isInstanced)
{
	this->isInstanced = isInstanced;
}

bool Object::GetIsInstanced() const
{
	return isInstanced;
}

// --- Static Functions ---

std::vector<Object*> Object::levelObjects;
std::vector<Object*> Object::enviormentObjects;

// maybe temp
void Object::DropPtr(Object* pObject)
{
	levelObjects.erase(std::remove(levelObjects.begin(), levelObjects.end(), pObject), levelObjects.end());
	enviormentObjects.erase(std::remove(enviormentObjects.begin(), enviormentObjects.end(), pObject), enviormentObjects.end());
}

void Object::EmptyObjectLists()
{
	levelObjects.clear();
	enviormentObjects.clear();
}

const std::vector<Object*>& Object::GetLevelObjects()
{
	return levelObjects;
}

const std::vector<Object*>& Object::GetEnviormentObjects()
{
	return enviormentObjects;
}