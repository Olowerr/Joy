#include "ObjParser.h"
ObjParser::ObjParser()
{
}

ObjParser::~ObjParser()
{
}

bool ObjParser::LoadObj(std::vector<VertexHolder>& vertsHolder, std::vector<int>& vertices, std::vector<Object>& objectVect)
{
	Vertices vtxInfo;
	std::string fileInfo;
	std::ifstream reader;

	int ObjectCounter = 0;
	int idxS = 0;
	int idxC = 0;
	int ObjectIdx = objectVect.size() - 1;
	bool skipFirstO = false;
	bool skipFirstMtl = false;
	int verticeCounter = 0;

	float vtx[3];
	float vtxUV[2];
	float vtxN[3];

	int faceP[3];
	int faceT[3];
	int faceN[3];

	reader.open("LE_FILE_MAN");
	if (!reader.is_open())
	{
		std::cerr << "Could not open obj file" << std::endl;
		return false;
	}

	while (!reader.eof())
	{
		reader >> fileInfo;
		if (fileInfo == "o")
		{
			ObjectCounter++;
		}
	}

	for (int i = 0; i < ObjectCounter; i++)
	{
		objectVect.emplace_back();
	}

	reader.clear();
	reader.seekg(0);
	while (reader >> fileInfo)
	{
		if (fileInfo == "o")
		{
			if (skipFirstO)
			{
				objectVect[ObjectIdx].setIdxC(idxC);
			}
			ObjectIdx++;
			skipFirstO = true;
			skipFirstMtl = false;
		}
		if (fileInfo == "v")
		{
			reader >> vtx[0];
			reader >> vtx[1];
			reader >> vtx[2];
			vtxInfo.vertex.emplace_back(vtx);
		}
		if (fileInfo == "vt")
		{
			reader >> vtxUV[0];
			reader >> vtxUV[1];
			vtxUV[1] = 1 - vtxUV[1];
			vtxInfo.vertexUV.emplace_back(vtxUV);
		}
		if (fileInfo == "vn")
		{
			reader >> vtxN[0];
			reader >> vtxN[1];
			reader >> vtxN[2];
			vtxInfo.vertexNormal.emplace_back(vtxN);
		}
		if (fileInfo == "usemtl")
		{
			idxS += idxC;
			objectVect[ObjectIdx].setIdxS(idxS);
			reader >> fileInfo;
			objectVect[ObjectIdx].setMtrlName(fileInfo);
			if (skipFirstMtl)
			{
				objectVect[ObjectIdx].setIdxC(idxC);
			}
			idxC = 0;
			skipFirstMtl = true;
		}
		if (fileInfo == "f")
		{
			for (int i = 0; i < 3; i++)
			{
				reader >> faceP[i];
				reader.ignore(1);
				reader >> faceT[i];
				reader.ignore(1);
				reader >> faceN[i];
				vertsHolder.emplace_back(vtxInfo.vertex[faceP[i] - 1],
					vtxInfo.vertexNormal[faceN[i] - 1],
					vtxInfo.vertexUV[faceT[i] - 1]);
				vertices.emplace_back(verticeCounter++);
				idxC++;
			}
		}
	}
	objectVect[ObjectIdx].setIdxC(idxC);
	reader.close();
	return true;
}

bool ObjParser::LoadMtl(std::vector<Material>& material)
{
	std::string fileInfo;
	std::ifstream reader;
	MaterialValues mtrlValues;

	bool defVal[9] = {};
	defVal[8] = true;
	float nss;
	float kas[3];
	float kds[3];
	float kss[3];
	std::string mapKds;
	std::string mapKss;
	std::string mapKas;
	int ill;
	int mtrlC = material.size() - 1;
	int amountMtrl = 0;

	reader.open("LE_FILE_MAN");
	if (!reader.is_open())
	{
		std::cerr << "Could not open mtl file" << std::endl;
		return false;
	}

	while (!reader.eof())
	{
		reader >> fileInfo;
		if (fileInfo == "newmtl")
		{
			amountMtrl++;
		}
	}

	for (int i = 0; i < amountMtrl; i++)
	{
		material.emplace_back();
	}

	reader.clear();
	reader.seekg(0);

	while (fileInfo != "newmtl")
	{
		reader >> fileInfo;
	}
	while (reader >> fileInfo)
	{
		if (defVal[8])
		{
			mtrlC++;
			material[mtrlC].setMtrlName(fileInfo);
			defVal[8] = false;
		}

		while (!defVal[8])
		{
			if (fileInfo == "Ns")
			{
				reader >> nss;
				material[mtrlC].setNsVal(nss);
				defVal[0] = true;
			}
			if (fileInfo == "Ka")
			{
				reader >> kas[0];
				reader >> kas[1];
				reader >> kas[2];
				mtrlValues.ka = { kas[0], kas[1], kas[2] };
				material[mtrlC].setKaVal(mtrlValues.ka);
				defVal[1] = true;
			}
			if (fileInfo == "Kd")
			{
				reader >> kds[0];
				reader >> kds[1];
				reader >> kds[2];
				mtrlValues.kd = { kds[0], kds[1], kds[2] };
				material[mtrlC].setKdVal(mtrlValues.kd);
				defVal[2] = true;
			}
			if (fileInfo == "Ks")
			{
				reader >> kss[0];
				reader >> kss[1];
				reader >> kss[2];
				mtrlValues.ks = { kss[0], kss[1], kss[2] };
				material[mtrlC].setKsVal(mtrlValues.ks);
				defVal[3] = true;
			}
			if (fileInfo == "illum")
			{
				reader >> ill;
				material[mtrlC].setIllumVal(ill);
				defVal[4] = true;
			}
			if (fileInfo == "map_Kd")
			{
				reader >> mapKds;
				material[mtrlC].setKd(mapKds);
				defVal[5] = true;
			}
			if (fileInfo == "map_Ks")
			{
				reader >> mapKss;
				material[mtrlC].setKs(mapKss);
				defVal[6] = true;
			}
			if (fileInfo == "map_Ka")
			{
				reader >> mapKas;
				material[mtrlC].setKa(mapKas);
				defVal[7] = true;
			}
			if (fileInfo == "newmtl")
			{
				if (!defVal[0])
				{
					material[mtrlC].setNsVal(18.0f);
				}
				if (!defVal[1])
				{
					DirectX::XMFLOAT3 temp = { 0.7f, 0.7f, 0.7f };
					material[mtrlC].setKaVal(temp);
				}
				if (!defVal[2])
				{
					DirectX::XMFLOAT3 temp = { 0.5f, 0.5f, 0.5f };
					material[mtrlC].setKdVal(temp);
				}
				if (!defVal[3])
				{
					DirectX::XMFLOAT3 temp = { 0.5f, 0.5f, 0.5f };
					material[mtrlC].setKsVal(temp);
				}
				if (!defVal[4])
				{
					material[mtrlC].setIllumVal(0);
				}
				if (!defVal[5])
				{
					material[mtrlC].setKd("Default_Textures_Man");
				}
				if (!defVal[6])
				{
					material[mtrlC].setKs("Default_Textures_Man");
				}
				if (!defVal[7])
				{
					material[mtrlC].setKa("Default_Textures_Man");
				}
				for (int i = 0; i < 8; i++)
				{
					defVal[i] = false;
				}
				defVal[8] = true;
			}
			if (!defVal[8])
			{
				reader >> fileInfo;
			}
			if (reader.eof())
			{
				if (!defVal[0])
				{
					material[mtrlC].setNsVal(18.0f);
				}
				if (!defVal[1])
				{
					DirectX::XMFLOAT3 temp = { 0.5f, 0.5f, 0.5f };
					material[mtrlC].setKaVal(temp);
				}
				if (!defVal[2])
				{
					DirectX::XMFLOAT3 temp = { 0.5f, 0.5f, 0.5f };
					material[mtrlC].setKdVal(temp);
				}
				if (!defVal[3])
				{
					DirectX::XMFLOAT3 temp = { 0.5f, 0.5f, 0.5f };
					material[mtrlC].setKsVal(temp);
				}
				if (!defVal[4])
				{
					material[mtrlC].setIllumVal(0);
				}
				if (!defVal[5])
				{
					material[mtrlC].setKd("Default_Textures_Man");
				}
				if (!defVal[6])
				{
					material[mtrlC].setKs("Default_Textures_Man");
				}
				if (!defVal[7])
				{
					material[mtrlC].setKa("Default_Textures_Man");
				}
				defVal[8] = true;
			}
		}
	}
	reader.close();
	return true;
}