#pragma once
#include <vector>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include "StoredData.h"

#include "Backend.h"

struct DiffTextures
{
	DiffTextures(std::string path, ID3D11ShaderResourceView* textureSRV)
		:path(path), textureSRV(textureSRV) {};
	DiffTextures() = default;
	std::string path;
	ID3D11ShaderResourceView* textureSRV;
};

class Mesh
{
public:
	static const UINT Stirde = sizeof(JOY::Vertex);
	static const UINT Offset = 0;

	Mesh() = default;
	~Mesh() = default;
	void Shutdown()
	{
		if (vertexBuffer)
			vertexBuffer->Release();
		if (indexBuffer)
			indexBuffer->Release();
	}

	//private:
	std::string name;

	UINT indexCount;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	ID3D11ShaderResourceView* diffuseTextureSRV;

	DirectX::BoundingBox bBox;

	std::vector<Mesh*> children;
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
	void UnloadJoy();
	void LoadMenuObjects();
	void LoadEasyObjects();
	void UnloadMeshes();
	void UnloadDataBase();

	Mesh* GetMeshByName(const std::string& name);

	// ptrs or reference? ( nullptr or ERROR mesh? )
	Mesh* GetMesh(UINT index);
	size_t GetMeshCount() const;

	Mesh* GetObjMesh(UINT index);
	size_t GetObjMeshCount() { return MeshCount; }

	Mesh joy[3];
	DiffTextures joyDiff;

private:

	std::vector<DiffTextures> diffTextures;
	bool sameTexture;

	const std::string meshPath = "../Resources/Meshes/";
	static const UINT MeshCount = 26;
	Mesh objMeshes[MeshCount];
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
		"Future_Bench.obj",				//10
		"MenuScene/MenuIsland.obj",		//11
		"MenuScene/Portal.obj",			//12
		"MenuScene/Book.obj",			//13
		"MenuScene/Bottle.obj",			//14
		"MenuScene/CompScreen.obj",		//15
		"MenuScene/Computer.obj",		//16
		"MenuScene/FlaskStand.obj",		//17
		"MenuScene/Keyboard.obj",		//18
		"MenuScene/MenuTable.obj",		//19
		"MenuScene/Mouse.obj",			//20
		"MenuScene/SmallEngine.obj",	//21
		"MenuScene/Stair1.obj",			//22
		"MenuScene/Stair2.obj",			//23
		"MenuScene/Stair3.obj",			//24
		"MenuScene/TV.obj"				//25
	};

	std::vector<Mesh*> meshes;
	const std::string tastPath = "../Resources/JoyFiles/"; //  /MenuStuff
	static const UINT MenuCount = 4;
	const std::string MenuFiles[MenuCount] =
	{
		//"Blockout_Test.joy" // 4 meshes
		"Portal.joy",
		"Arrow.joy",
		"Carpet.joy",
		//"TestSceneUpdated.joy"
		"MainMenuFix.joy"
		//"MenuScene.joy"
	};

	static const UINT EasyCount = 3;
	const std::string EasyFiles[EasyCount] =
	{
		// menu .joy files
		"Portal.joy",
		"LevelInst3.joy",
		"PMesh.joy"
	};

	void import(UINT index);
	void import(const std::string& filePath);

};