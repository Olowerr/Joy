#include "Material.h"
Material::Material()
{
}

Material::~Material()
{
}

void Material::setKd(std::string mapKd)
{
	texKd = mapKd;
}

void Material::setKs(std::string mapKs)
{
	texKs = mapKs;
}

void Material::setKa(std::string mapKa)
{
	texKa = mapKa;
}

void Material::setMtrlName(std::string name)
{
	mtrlName = name;
}

void Material::setNsVal(float nsVal)
{
	ns = nsVal;
}

void Material::setKaVal(DirectX::XMFLOAT3 kaVal)
{
	ka = kaVal;
}

void Material::setKdVal(DirectX::XMFLOAT3 kdVal)
{
	kd = kdVal;
}

void Material::setKsVal(DirectX::XMFLOAT3 ksVal)
{
	ks = ksVal;
}

void Material::setIllumVal(int illumVal)
{
	illum = illumVal;
}

void Material::destroy()
{
	for (int i = 0; i < 3; i++)
	{
		srv[i]->Release();
	}
	nsValBuffer->Release();
}

std::string& Material::getKd()
{
	return texKd;
}

std::string& Material::getKs()
{
	return texKs;
}

std::string& Material::getKa()
{
	return texKa;
}

std::string& Material::getMtrlName()
{
	return mtrlName;
}

float& Material::getNsValue()
{
	return ns;
}

DirectX::XMFLOAT3& Material::getKaValue()
{
	return ka;
}

DirectX::XMFLOAT3& Material::getKdValue()
{
	return kd;
}

DirectX::XMFLOAT3& Material::getKsValue()
{
	return ks;
}

int Material::getIllumValue()
{
	return illum;
}

ID3D11ShaderResourceView** Material::getSrv()
{
	return srv;
}

ID3D11Buffer** Material::getNsBuffer()
{
	return &nsValBuffer;
}
