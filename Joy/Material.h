#pragma once
#include <string>
#include <vector>
#include <DirectXMath.h>
#include <d3d11.h>

class Material
{
public:

	Material();
	~Material();

	void setKd(std::string mapKd);
	void setKs(std::string mapKs);
	void setKa(std::string mapKa);
	void setMtrlName(std::string name);
	void setNsVal(float nsVal);
	void setKaVal(DirectX::XMFLOAT3 kaVal);
	void setKdVal(DirectX::XMFLOAT3 kdVal);
	void setKsVal(DirectX::XMFLOAT3 ksVal);
	void setIllumVal(int illumVal);
	void destroy();
	std::string& getKd();
	std::string& getKs();
	std::string& getKa();
	std::string& getMtrlName();
	float& getNsValue();
	DirectX::XMFLOAT3& getKaValue();
	DirectX::XMFLOAT3& getKdValue();
	DirectX::XMFLOAT3& getKsValue();
	int getIllumValue();
	ID3D11ShaderResourceView** getSrv();
	ID3D11Buffer** getNsBuffer();

private:

	std::string texKd;
	std::string texKs;
	std::string texKa;
	std::string mtrlName;
	float ns;
	DirectX::XMFLOAT3 ka;
	DirectX::XMFLOAT3 kd;
	DirectX::XMFLOAT3 ks;
	int illum;

	ID3D11ShaderResourceView* srv[3];
	ID3D11Buffer* nsValBuffer;
};
