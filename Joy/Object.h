#pragma once
#include "Transform.h"
#include "MeshStorage.h"
#include <DirectXCollision.h>

class Object : public Transform
{
public:

	// Make protected, should be for Joy ONLY. bool levelObject here is temporary before fix
	Object(Mesh* mesh, bool levelObject);
	Object(Mesh* mesh, bool levelObject, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot = DirectX::XMFLOAT3(), FLOAT scale = 1.f);
	virtual void Shutdown() override;
	virtual ~Object();

	void CheckBB();

	void Draw();
	void DrawGeometry();

	void Translate(const DirectX::XMVECTOR& movement) override;
	void Translate(FLOAT X, FLOAT Y, FLOAT Z) override;
	void SetPosition(const DirectX::XMVECTOR& position) override;
	void SetPosition(FLOAT X, FLOAT Y, FLOAT Z) override;

	void Scale(FLOAT amount) override;
	void SetScale(FLOAT Scale) override;

	const DirectX::BoundingBox& GetBoundingBox() const;

	Mesh* GetMesh();
	ID3D11ShaderResourceView** GetLightMapSRV();

	const bool IsLevelObject;
<<<<<<< HEAD
	void SetInstanced(bool isInstanced);
=======
	void SetIsInstanced(bool isInstanced);
>>>>>>> master
	bool GetIsInstanced() const;

private:
	Mesh* mesh;
	DirectX::BoundingBox bBox;
	//hej
	ID3D11ShaderResourceView* lightMap; // no use in joy
	bool isInstanced;


	// "Global" arrays of all objects
	static std::vector<Object*> levelObjects;
	static std::vector<Object*> enviormentObjects;
public:
	static void DropLevelPtr(Object* pObject); // maybe temp
	static const std::vector<Object*>& GetLevelObjects();
	static const std::vector<Object*>& GetEnviormentObjects();
};