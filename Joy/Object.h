#pragma once
#include "Transform.h"
#include "MeshStorage.h"

class Object : public Transform
{
public:

	Object(Mesh* mesh);
	Object(DX::XMFLOAT3 pos, DX::XMFLOAT3 rot, FLOAT scale, Mesh* mesh);
	virtual void Shutdown() override;

	void Draw();

	Mesh* GetMesh();

private:
	Mesh* mesh;
	//hej

};