#include "Object.h"
Object::Object()
	:idxS(0), idxC(0), mtrlName()
{
	DirectX::XMMATRIX tempMtrx = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	DirectX::XMStoreFloat4x4(&wrldMtrx, tempMtrx);
}

Object::~Object()
{
}

bool Object::CreateWorldMtrxBuffer(ID3D11Device* device)
{
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT4X4);
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &wrldMtrx;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	HRESULT hr = device->CreateBuffer(&bufferDesc, &data, &worldMtrxBuffer);
	return SUCCEEDED(hr);
}

void Object::setIdxS(int idxStart)
{
	idxS.push_back(idxStart);
}

void Object::setIdxC(int idxCount)
{
	idxC.push_back(idxCount);
}

void Object::setMtrlName(std::string mtrlNames)
{
	mtrlName.push_back(mtrlNames);
}

std::vector<int>& Object::getIdxS()
{
	return idxS;
}

std::vector<int>& Object::getIdxC()
{
	return idxC;
}

std::vector<std::string>& Object::getMtrlName()
{
	return mtrlName;
}

DirectX::XMFLOAT4X4& Object::getWorldMtrx()
{
	return wrldMtrx;
}

void Object::setWorldMtrx(DirectX::XMMATRIX& wrldMtrx)
{
	DirectX::XMStoreFloat4x4(&this->wrldMtrx, wrldMtrx);
}

ID3D11Buffer*& Object::getWorldMtrxBuffer()
{
	return worldMtrxBuffer;
}

void Object::releaseBuffer()
{
	worldMtrxBuffer->Release();
}
