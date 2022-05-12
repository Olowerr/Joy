#pragma once
#include <vector>
#include <DirectXMath.h>
#include <DirectXCollision.h>





#include "StoredData.h"

#include "Backend.h"

class Mesh
{
public:
	static const UINT Stirde = sizeof(JOY::Vertex);
	static const UINT Offset = 0;
	
	Mesh() = default;
	void Shutdown()
	{
		diffuseTextureSRV->Release();
		vertexBuffer->Release();
	}

	void Bind();
	void BindGeometry();

//private:
	UINT indexCount;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	
	ID3D11ShaderResourceView* diffuseTextureSRV;
	
	DirectX::BoundingBox bBox;
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

	void LoadAllObj();
	void UnloadObjMeshes();

	void LoadMenuObjects();
	void LoadEasyObjects();
	void UnloadMeshes();
	void UnloadDataBase();
	
	
	// ptrs or reference? ( nullptr or ERROR mesh? )
	Mesh* GetMesh(UINT index);
	size_t GetMeshCount() const;

	Mesh* GetObjMesh(UINT index);
	size_t GetObjMeshCount() { return MeshCount; }

private:

	const std::string meshPath = "../Resources/Meshes/";
	static const UINT MeshCount = 11;
	Mesh objMeshes[MeshCount];
	const std::string meshNames[MeshCount] =
	{
		"Joy.obj",
		"Kub.obj",
		"Ground.obj",
		"Portal.obj",
		"Wall.obj",
		"Wall2.obj",
		"sphere.obj",
		"FirstTree.obj",
		"HighscoreScreen.obj",
		"Frame.obj",
		"Future_Bench.obj"
	};

	std::vector<Mesh*> meshes;

	const std::string tastPath = "../Resources/JoyFiles/"; //  /MenuStuff
	static const UINT MenuCount = 1;
	const std::string MenuFiles[MenuCount] =
	{
		//"big3newnew.joy"
		"Blockout_Test.joy" // 4 meshes
	};

	static const UINT EasyCount = 1;
	const std::string EasyFiles[MenuCount] =
	{
		// menu .joy files
	};




	void import(UINT index);
	void import(const std::string& filePath);
};