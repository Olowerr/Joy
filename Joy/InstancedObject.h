#pragma once
#include "Transform.h"
#include "MeshStorage.h"
#include <DirectXCollision.h>

class InstancedObject
{
public:

	InstancedObject();
	~InstancedObject();

	ID3D11Buffer* vertexBuffer;
	//ID3D11Buffer* indexBuffer; 
	ID3D11ShaderResourceView* transformSRV;
	ID3D11ShaderResourceView* mtl;
	//ID3D11ShaderResourceView* lightMapsSRV;
	UINT indexCount;
	UINT instanceCount;

	void Shutdown();

private:

};