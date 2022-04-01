#include "Object.h"
Object::Object(Mesh* mesh)
	:mesh(mesh)
{

}

Object::Object(DX::XMFLOAT3 pos, DX::XMFLOAT3 rot, FLOAT scale, Mesh* mesh)
	:Transform(pos, rot, scale), mesh(mesh)
{
}

void Object::Shutdown()
{
	Transform::Shutdown();
}

void Object::Draw()
{
	ID3D11DeviceContext* dc = Backend::Get().GetDeviceContext();

	dc->IASetVertexBuffers(0, 1, &mesh->vertexBuffer, &Mesh::Stirde, &Mesh::Offset);
	dc->VSSetConstantBuffers(0, 1, GetTransformBuffer());
	dc->Draw(mesh->vertexCount, 0);
}
