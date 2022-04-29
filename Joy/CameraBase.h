#pragma once
#include <DirectXMath.h>
#include "Backend.h"

__declspec(align(16))
class Camera
{
public:
	Camera();
	virtual void Shutdown();

	virtual void UpdateCam() = 0;
	virtual void SetView() = 0;

	ID3D11Buffer* const* GetMatrixBuffer() const;


protected:
	DirectX::XMFLOAT4X4 viewProjMatrix;
	ID3D11Buffer* camMatrixBuffer;


	// Overloaded operators
public:
	void* operator new(size_t i)
	{
		return _aligned_malloc(i, 16);
	}
	void operator delete(void* ptr)
	{
		_aligned_free(ptr);
	}

};