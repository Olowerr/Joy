#pragma once
#include "Transform.h"
#include "MeshStorage.h"

class Object : public Transform
{
public:

	Object(Mesh* mesh);
	Object(Mesh* mesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, FLOAT scale);
	virtual void Shutdown() override;

	void Draw();

	Mesh* GetMesh();

private:
	Mesh* mesh;
	//hej

};