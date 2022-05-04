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

	void Draw();

	const UINT Capacity;

private:
	Mesh* mesh;
	ID3D11ShaderResourceView* transformSRV;
	//ID3D11ShaderResourceView* lightMapsSRV;
	UINT instanceCount;

	Object** ppObjects;
};