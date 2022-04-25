#include "Object.h"
Object::Object(Mesh* mesh)
	:mesh(mesh)
{
	bBox = mesh->bBox;
}

Object::Object(Mesh* mesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, FLOAT scale)
	:Transform(pos, rot, scale), mesh(mesh)
{
	bBox = mesh->bBox;
	bBox.Center.x += pos.x;
	bBox.Center.y += pos.y;
	bBox.Center.z += pos.z;
	bBox.Extents.x *= scale;
	bBox.Extents.y *= scale;
	bBox.Extents.z *= scale;

}

void Object::Shutdown()
{
	Transform::Shutdown();
}

void Object::Draw()
{
	ID3D11DeviceContext* dc = Backend::GetDeviceContext();

	dc->IASetVertexBuffers(0, 1, &mesh->vertexBuffer, &Mesh::Stirde, &Mesh::Offset);
	dc->VSSetConstantBuffers(0, 1, GetTransformBuffer());
	dc->PSSetShaderResources(0, 1, &mesh->diffuseTextureSRV);
	dc->Draw(mesh->vertexCount, 0);
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
	bBox.Extents.x = mesh->bBox.Center.x * Scale;
	bBox.Extents.y = mesh->bBox.Center.y * Scale;
	bBox.Extents.z = mesh->bBox.Center.z * Scale;
}

const DirectX::BoundingBox& Object::GetBoundingBox() const
{
	return bBox;
}

Mesh* Object::GetMesh()
{
	return mesh;
}

ID3D11ShaderResourceView** Object::GetLightMap()
{
	return &lightMap;
}
