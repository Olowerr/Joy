#pragma once
#include <fbxsdk.h>
#include <vector>
#include <fstream>

#define NOMINMAX
#include <windows.h>

#include "BaseHeader.h"
#include "JOY.h"


typedef const std::string& cStringR;
typedef unsigned int UINT;

struct NamedKeyframes
{
	NamedKeyframes() = default;

	JoyString jointName;
	KeyFrame frame;
};

struct MeshDeformer
{
	JoyString shapeName;
	std::vector<MorphFrame> frames;
	std::vector<MorphVTX> vertices;
};

struct MeshData
{
	MeshData() = default;
	MeshData(const std::string& name) 
		:name(name)
	{
	}
	~MeshData()
	{
		for (PropertyBase* prop : props)
			delete prop;
	}

	JoyString name;

	JoyString mtlName = "None";
	
	JoyString parent = "None";

	std::vector<JoyString> childrenNames;

	int skeletonId = -1;
	int morphId = -1;
	int propertyId = -1;

	std::vector<Vertex> verts;
	std::vector<BiNormals> biNorms;

	std::vector<int> idx;

	std::vector<JoyString> jointNames;

	std::vector<int> weightVtxCounter;	
	std::vector<WeightVTX> weightVtx;	
	std::vector<int> vtxCPIdx;			
	
	std::vector<MeshDeformer*> deformers;
	
	std::vector<Joint> joints;

	std::vector<PropertyBase*> props;
};


class Writer
{
public:
	static MeshData* FindMesh(cStringR name)
	{
		for (int i = 0; i < meshes.size(); i++)
		{
			if (meshes[i].name == name)
				return &meshes[i];
		}

		return nullptr;
	}
	static Group* FindGroup(cStringR name)
	{
		for (size_t i = 0; i < groups.size(); i++)
		{
			if (groups.at(i).name == name)
				return &groups.at(i);
		}

		return nullptr;
	}

	static void MakeUnique(MeshData& mesh);
	static void AddProp(cStringR meshName, FbxProperty& prop);

	static void WriteFile(cStringR filePath);

	static void AddMesh(FbxNode* pNode)
	{
		meshes.emplace_back(pNode->GetName());
		FbxNode* pParent = pNode->GetParent();
		if (pParent->GetNameOnly() != FbxString("RootNode"))
			meshes.back().parent = pParent->GetName();

		for (int i = 0; i < pNode->GetChildCount(); i++)
		{
			meshes.back().childrenNames.emplace_back(pNode->GetChild(i)->GetName());
		}

	}

	// Vertex Data
	static void AddVertex(cStringR meshName, int idx);						
	static void SetVertexPosition(cStringR meshName, FbxVector4 vector);	
	static void SetVertexUV(cStringR meshName, FbxVector2 vector);			
	static void SetVertexNormal(cStringR meshName, FbxVector4 vector);		
																			
	static void AddVertexBiNormals(cStringR meshName);						
	static void SetVertexTangent(cStringR meshName, FbxVector4 vector);		
	static void SetVertexBiTangent(cStringR meshName, FbxVector4 vector);	

	// Material Data
	static void AddMaterial(cStringR name);
	static void SetMaterial(cStringR meshName, cStringR mtlName);
	static void SetMatAmbient(float ambient1, float ambient2, float ambient3);
	static void SetMatDiffuse(float diffuse1, float diffuse2, float diffuse3);
	static void SetMatSpecular(float specular1, float specular2, float specular3);
	static void SetMatIdx(int idx);

	// Texture Data
	static void AddTexture(const char* path);
	static bool SetTexture(const char* materialName, const char* name);
	static void CopyTextureFile(const char* texturePath);

	// Camera Data
	static void AddCamera();
	static void SetCameraPos(FbxDouble3 camPos);
	static void SetUpVector(FbxDouble3 upVec);
	static void SetFwVector(FbxDouble3 fwVec);
	static void SetFOV(float FOV);
	static void SetNearPlane(float nearPlane);
	static void SetFarPlane(float farPlane);

	// Morph Data
	static void SetMorphFrames(cStringR meshName, FbxAnimCurve* pCurve, const char* shapeName);
	static void SetMorphVTX(FbxGeometry* pGeometry, FbxShape* shape, const char* shapeName);

	// Light Data
	static void AddLight();
	static void SetLightColor(float col1, float col2, float col3);
	static void SetLightPos(FbxDouble3 pos);
	static void SetLightDir(FbxDouble3 rot);

	// Keyframe Data
	static void SetKeyframeValues(float value, std::string checker, int timeStamp, int idx, int idxAmount, std::string jointName);
	static void KeyframeTrue();
	static bool isKeyframeTrue();

	// Joint Data
	static void AddBindPose(cStringR name, FbxNode* pNode);
	static void SetBindPose(float rowValue1, float rowValue2, float rowValue3, float rowValue4, int idx, cStringR name);
	static void CalcNumFrames();

	static void AddJoint(cStringR meshName, cStringR name);
	static void SetWeight(cStringR meshname, int boneIdx, int idxC, int* pIdx, double* pWeights);
	static void NormalizeWeights(cStringR meshName);
	static void CreateWeightVtx(FbxNode* pNode);

	// Group Data
	static void AddGroup(cStringR name);
	static void SetGroupTranslation(FbxDouble3 translations);
	static void SetGroupRotation(FbxDouble3 rotations);
	static void SetGroupScale(FbxDouble3 scales);
	static bool isGroupTrue();

	// Animation Time Data
	static void SetAnimationTime(float frameRate, int start, int end);

private:
	
	static std::vector<MeshData> meshes;
	static std::vector<Material> materials;
	static std::vector<int> matIdx;
	static std::vector<Camera> cameras;
	static std::vector<Light> lights;
	static std::vector<NamedKeyframes> keyframes;
	static std::vector<Group> groups;
	static AnimationData aniData;
	static bool isGroup;
	static int bindPoseIdx;
	static bool isKeyframe;
	static int posIdx;
	static int dirIdx;
	static int instCounter;

};