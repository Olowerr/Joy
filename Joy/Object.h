#pragma once
#include "Transform.h"
#include "MeshStorage.h"
#include <DirectXCollision.h>

class Object : public Transform
{
public:

	Object(Mesh* mesh);
	Object(Mesh* mesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, FLOAT scale);
	virtual void Shutdown() override;

	void Draw();

	void Translate(const DirectX::XMVECTOR& movement) override;
	void Translate(FLOAT X, FLOAT Y, FLOAT Z) override;
	void SetPosition(const DirectX::XMVECTOR& position) override;
	void SetPosition(FLOAT X, FLOAT Y, FLOAT Z) override;

	void Scale(FLOAT amount) override;
	void SetScale(FLOAT Scale) override;

	const DirectX::BoundingBox& GetBoundingBox() const;

	Mesh* GetMesh();
	ID3D11ShaderResourceView** GetLightMap();

private:
	Mesh* mesh;
	DirectX::BoundingBox bBox;
	//hej
	ID3D11ShaderResourceView* lightMap; // no use in joy
};