#include "Object.h"
#include <iostream>

Object::Object(Mesh* mesh, bool levelObject)
	:mesh(mesh), lightMap(nullptr), IsLevelObject(levelObject), isInstanced(false)
{
	bBox = mesh->bBox;

	if (levelObject)
		levelObjects.emplace_back(this);
	else
		enviormentObjects.emplace_back(this);
}

Object::Object(Mesh* mesh, bool levelObject, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, FLOAT scale)
	:Transform(pos, rot, scale), mesh(mesh), lightMap(nullptr), IsLevelObject(levelObject), isInstanced(false)
{
	bBox = mesh->bBox;
	bBox.Center.x += pos.x;
	bBox.Center.y += pos.y;
	bBox.Center.z += pos.z;
	bBox.Extents.x *= scale;
	bBox.Extents.y *= scale;
	bBox.Extents.z *= scale;

	if (levelObject)
		levelObjects.emplace_back(this);
	else
		enviormentObjects.emplace_back(this);
}

void Object::Shutdown()
{
	Transform::Shutdown();

	if (lightMap)
		lightMap->Release();
}

Object::~Object()
{
	std::cout << "called\n";
}

void Object::CheckBB()
{
	bBox = mesh->bBox;
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
	bBox.Center.x += movement.m128_f32[0];
	bBox.Center.y += movement.m128_f32[1];
	bBox.Center.z += movement.m128_f32[2];
}

void Object::Translate(FLOAT X, FLOAT Y, FLOAT Z)
{
	Transform::Translate(X, Y, Z);
	bBox.Center.x += X;
	bBox.Center.y += Y;
	bBox.Center.z += Z;
}

void Object::SetPosition(const DirectX::XMVECTOR& position)
{
	Transform::SetPosition(position);
	bBox.Center.x = mesh->bBox.Center.x + position.m128_f32[0];
	bBox.Center.y = mesh->bBox.Center.y + position.m128_f32[1];
	bBox.Center.z = mesh->bBox.Center.z + position.m128_f32[2];
}

void Object::SetPosition(FLOAT X, FLOAT Y, FLOAT Z)
{
	Transform::SetPosition(X, Y, Z);
	bBox.Center.x = mesh->bBox.Center.x + X;
	bBox.Center.y = mesh->bBox.Center.y + Y;
	bBox.Center.z = mesh->bBox.Center.z + Z;
}

void Object::Scale(FLOAT amount)
{
	Transform::Scale(amount);
	bBox.Extents.x *= amount;
	bBox.Extents.y *= amount;
	bBox.Extents.z *= amount;
}

void Object::SetScale(FLOAT Scale)
{
	Transform::SetScale(Scale);
	/*bBox.Extents.x = mesh->bBox.Extents.x * Scale;
	bBox.Extents.y = mesh->bBox.Extents.y * Scale;
	bBox.Extents.z = mesh->bBox.Extents.z * Scale;*/

	using namespace DirectX;

	XMVECTOR objectPos = DirectX::XMLoadFloat3(&GetPosition());
	XMVECTOR scaledDelta = (DirectX::XMLoadFloat3(&bBox.Center) - objectPos) * Scale;

	XMStoreFloat3(&bBox.Center, objectPos + scaledDelta);

	bBox.Extents.x = mesh->bBox.Extents.x * Scale;
	bBox.Extents.y = mesh->bBox.Extents.y * Scale;
	bBox.Extents.z = mesh->bBox.Extents.z * Scale;
}

const DirectX::BoundingBox& Object::GetBoundingBox() const
{
	return bBox;
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
void Object::DropLevelPtr(Object* pObject)
{
	levelObjects.erase(std::remove(levelObjects.begin(), levelObjects.end(), pObject), levelObjects.end());
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