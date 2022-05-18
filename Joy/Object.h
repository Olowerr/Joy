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

	virtual void Draw();
	void DrawGeometry();

	void Translate(const DirectX::XMVECTOR& movement) override;
	void Translate(FLOAT X, FLOAT Y, FLOAT Z) override;
	void SetPosition(const DirectX::XMVECTOR& position) override;
	void SetPosition(FLOAT X, FLOAT Y, FLOAT Z) override;

	void AddBBox();
	void AddBBox(DirectX::XMFLOAT3 center, DirectX::XMFLOAT3 extents);
	void SetBBox(int bBoxIndex, DirectX::XMFLOAT3 pos, FLOAT scale);
	void SetBBox(int bBoxIndex, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 extents);
	void RemoveBBox(int bBoxIndex);

	void Scale(FLOAT amount) override;
	void SetScale(FLOAT Scale) override;

	const DirectX::BoundingBox& GetBoundingBox(int index) const;
	const int GetNumBboxes();

	Mesh* GetMesh();
	ID3D11ShaderResourceView** GetLightMapSRV();

	const bool IsLevelObject;
	void SetIsInstanced(bool isInstanced);
	bool GetIsInstanced() const;

private:
	Mesh* mesh;
	std::vector<DirectX::BoundingBox> bBox;

	ID3D11ShaderResourceView* lightMap; // no use in joy
	bool isInstanced;

	// "Global" arrays of all objects
	static std::vector<Object*> levelObjects;
	static std::vector<Object*> enviormentObjects;
public:
	static void DropPtr(Object* pObject); // maybe temp
	static void EmptyObjectLists();
	static const std::vector<Object*>& GetLevelObjects();
	static const std::vector<Object*>& GetEnviormentObjects();


	void* operator new(size_t i)
	{
		return _aligned_malloc(i, 16);
	}
	void operator delete(void* ptr)
	{
		_aligned_free(ptr);
	}
};