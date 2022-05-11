#pragma once
#include "Object.h"

class InstancedObject
{
public:

	InstancedObject(UINT capacity = 5);
	~InstancedObject();
	void Shutdown();

	void AddObject(Object* pObject);
	bool Finalize();
	
	ID3D11ShaderResourceView** GetLightMaps();
	Object** GetObjects();
	UINT GetNumObjects();

	void Draw();

	const UINT Capacity;

private:
	Mesh* mesh;
	ID3D11ShaderResourceView* transformSRV;
	ID3D11ShaderResourceView* lightMapsSRV;
	UINT instanceCount;

	Object** ppObjects;

	void ShrinkToFit();

	static std::vector<InstancedObject*> levelObjects;
	static std::vector<InstancedObject*> enviormentObjects;
public:
	static const std::vector<InstancedObject*>& GetLevelInstancedObjects();
	static const std::vector<InstancedObject*>& GetEnviormentInstancedObjects();
};