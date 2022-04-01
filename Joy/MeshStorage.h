#pragma once
#include <vector>
#include "Backend.h"
#include <DirectXMath.h>

struct Vertex
{
	Vertex(float* Pos, float* Uv, float* Norm)
		:pos(Pos), uv(Uv), normal(Norm)
	{ }
	Vertex(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 uv, DirectX::XMFLOAT3 norm)
		:pos(pos), uv(uv), normal(norm)
	{ }
	
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 normal;
};

struct Mesh
{
	static const UINT Stirde = sizeof(Vertex);
	static const UINT Offset = 0;

	ID3D11Buffer* vertexBuffer;
	UINT vertexCount;
};

class MeshStorage
{
public:
	MeshStorage();
	void Load();
	void Shutdown();

	Mesh* GetMesh(const std::string& name);
	Mesh* GetMesh(UINT index);

private:

	std::vector<Mesh*> meshes;

	const std::string meshPath = "../Resources/Meshes/";
	static const UINT MeshCount = 1;
	const std::string meshNames[MeshCount] =
	{
		"cube2.obj"
	};


	void Load(UINT index);
};