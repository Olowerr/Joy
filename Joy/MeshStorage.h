#pragma once
#include <vector>
#include <DirectXMath.h>
#include <DirectXCollision.h>

#include "Backend.h"


struct Vertex
{
	Vertex(float* Pos, float* Norm, float* Uv)
		:pos(Pos), normal(Norm), uv(Uv)
	{ }
	Vertex(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 norm, DirectX::XMFLOAT2 uv)
		:pos(pos), normal(norm), uv(uv)
	{ }
	
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;
};

struct Mesh
{
	static const UINT Stirde = sizeof(Vertex);
	static const UINT Offset = 0;
	DirectX::BoundingBox bBox;
	void Shutdown()
	{
		diffuseTextureSRV->Release();
		vertexBuffer->Release();
	}

	ID3D11ShaderResourceView* diffuseTextureSRV;

	ID3D11Buffer* vertexBuffer;
	//ID3D11Buffer* indexBuffer;
	UINT vertexCount; // index count
};

/*

	Most of this file is temporary, will change with .JOY lib
	Most of this file is temporary, will change with .JOY lib
	Most of this file is temporary, will change with .JOY lib
	Most of this file is temporary, will change with .JOY lib
	Most of this file is temporary, will change with .JOY lib
	Most of this file is temporary, will change with .JOY lib

*/

class TempMeshStorage
{
public:

	TempMeshStorage();
	~TempMeshStorage();

	void LoadAll();
	void UnLoadAll();

	// ptrs or reference? ( nullptr or ERROR mesh? )
	Mesh* GetMesh(const std::string& name);
	Mesh* GetMesh(UINT index);

private:

	const std::string meshPath = "../Resources/Meshes/";
	static const UINT MeshCount = 25;
	Mesh meshes[MeshCount];
	const std::string meshNames[MeshCount] =
	{
		"Joy.obj",						//0
		"Kub.obj",						//1
		"Ground.obj",					//2
		"Portal.obj",					//3
		"Wall.obj",						//4
		"Wall2.obj",					//5
		"sphere.obj",					//6
		"FirstTree.obj",				//7
		"HighscoreScreen.obj",			//8
		"Frame.obj",					//9
		"MenuScene/MenuIsland.obj",		//10
		"MenuScene/Portal.obj",			//11
		"MenuScene/CompScreen.obj",		//12
		"MenuScene/Computer.obj",		//13
		"MenuScene/Engine.obj",			//14
		"MenuScene/FlaskStand.obj",		//15
		"MenuScene/Keyboard.obj",		//16
		"MenuScene/Book.obj",			//17
		"MenuScene/MenuTable.obj",		//18
		"MenuScene/Mouse.obj",			//19
		"MenuScene/Bottle.obj",			//20
		"MenuScene/StairBlock1.obj",	//21
		"MenuScene/StairBlock2.obj",	//22
		"MenuScene/StairBlock3.obj",	//23
		"MenuScene/TV.obj"				//24
	};

	void import(UINT index);
};