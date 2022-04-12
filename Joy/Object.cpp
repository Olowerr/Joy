#include "Object.h"
Object::Object(Mesh* mesh)
	:mesh(mesh)
{

}

Object::Object(Mesh* mesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, FLOAT scale)
	:Transform(pos, rot, scale), mesh(mesh)
{
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

Mesh* Object::GetMesh()
{
	return mesh;
}
