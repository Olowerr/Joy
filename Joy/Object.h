#pragma once
#include <string>
#include <vector>
#include <DirectXMath.h>
#include <d3d11.h>

class Object
{
public:

	Object();

	~Object();

	bool CreateWorldMtrxBuffer(ID3D11Device* device);

	void setIdxS(int idxStart);
	void setIdxC(int idxCount);
	void setMtrlName(std::string mtrlNames);
	std::vector<int>& getIdxS();
	std::vector<int>& getIdxC();
	std::vector<std::string>& getMtrlName();
	DirectX::XMFLOAT4X4& getWorldMtrx();
	void setWorldMtrx(DirectX::XMMATRIX& wrldMtrx);
	ID3D11Buffer*& getWorldMtrxBuffer();

	void releaseBuffer();

	void Draw()
	{
		/*dc->set my vertexbuffer
			set my indexBuffer


		for each material
			set my material
			dc->DrawIndexed(start[i], count[i], 0)*/
	}

private:

	std::vector<int> idxS;
	std::vector<int> idxC;
	std::vector<std::string> mtrlName;
	DirectX::XMFLOAT4X4 wrldMtrx;
	ID3D11Buffer* worldMtrxBuffer;

};