#include "MeshStorage.h"

MeshStorage::MeshStorage()
{

}

void MeshStorage::Shutdown()
{
	for (Mesh* mesh : meshes)
	{
		mesh->vertexBuffer->Release();
		delete mesh;
	}
}


void MeshStorage::LoadAll()
{
	for (UINT i = 0; i < MeshCount; i++)
	{
		import(i);
	}
}

Mesh* MeshStorage::GetMesh(const std::string& name)
{
	for (UINT i = 0; i < MeshCount; i++)
	{
		if (name == meshNames[i])
			return meshes[i];
	}

	return nullptr;
}

Mesh* MeshStorage::GetMesh(UINT index)
{
	if (index >= meshes.size())
		return nullptr;

	return meshes[index];
}

void MeshStorage::import(UINT index)
{
	std::vector<DirectX::XMFLOAT3> pos;
	std::vector<DirectX::XMFLOAT2> uv;
	std::vector<DirectX::XMFLOAT3> norm;

	std::vector<Vertex> verts;

	std::string fileInfo;
	std::ifstream reader;

	float vtx[3];
	float vtxUV[2];
	float vtxN[3];

	int faceP;
	int faceT;
	int faceN;

	reader.open(meshPath + meshNames[index]);
	if (!reader.is_open())
		return;

	reader.clear();
	reader.seekg(0);
	while (reader >> fileInfo)
	{

		if (fileInfo == "v")
		{
			reader >> vtx[0];
			reader >> vtx[1];
			reader >> vtx[2];
			pos.emplace_back(vtx);
		}
		if (fileInfo == "vt")
		{
			reader >> vtxUV[0];
			reader >> vtxUV[1];
			vtxUV[1] = 1 - vtxUV[1];
			uv.emplace_back(vtxUV);
		}
		if (fileInfo == "vn")
		{
			reader >> vtxN[0];
			reader >> vtxN[1];
			reader >> vtxN[2];
			norm.emplace_back(vtxN);
		}
		//if (fileInfo == "usemtl")
		//{
		//	idxS += idxC;
		//	objectVect[ObjectIdx].setIdxS(idxS);
		//	reader >> fileInfo;
		//	objectVect[ObjectIdx].setMtrlName(fileInfo);
		//	if (skipFirstMtl)
		//	{
		//		objectVect[ObjectIdx].setIdxC(idxC);
		//	}
		//	idxC = 0;
		//	skipFirstMtl = true;
		//}
		if (fileInfo == "f")
		{
			for (int i = 0; i < 3; i++)
			{
				reader >> faceP;
				reader.ignore(1);
				reader >> faceT;
				reader.ignore(1);
				reader >> faceN;

				verts.emplace_back(	pos[faceP - 1],
									uv[faceT - 1],
									norm[faceN - 1]);

			}
		}
	}
	reader.close();

	meshes.emplace_back(new Mesh);
	
	meshes.back()->vertexCount = verts.size();
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = sizeof(Vertex) * verts.size();
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = verts.data();
	data.SysMemPitch = data.SysMemSlicePitch = 0;
	Backend::GetDevice()->CreateBuffer(&desc, &data, &meshes.back()->vertexBuffer);
}
