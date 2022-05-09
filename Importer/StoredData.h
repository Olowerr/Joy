#pragma once
#include "JOY.h"
#include "BaseHeader.h"
#include <vector>
#include <fstream>

struct MorphInfo
{
	std::vector<JOY::MorphFrame> morphFrame;
	std::vector<JOY::MorphVTX> morphVTX;
	JOY::MorphTargetInfo morphTargetInfo;
};

struct ObjectJointToho
{
	JOY::Joint jointInfo;
	std::vector<JOY::KeyFrame> keyFrames;
};

struct ObjectInfo
{
	std::vector<JOY::JoyString> children;

	ObjectHeader objHeader;

	std::vector<JOY::Vertex> vertex;
	std::vector<JOY::BiNormals> biNormal;
	std::vector<JOY::WeightVTX> weights;
	std::vector<int> indices;

	std::vector<MorphInfo> mInfo;

	std::vector<ObjectJointToho> joints;

};

struct MaterialInfo
{
	std::vector<JOY::Material> material;
};

struct LightInfo
{
	LightInfo()
		: lights{} {}

	std::vector<JOY::Light> lights;
};

struct CameraInfo
{
	CameraInfo()
		: cameras{} {}

	std::vector<JOY::Camera> cameras;
};

struct GroupInfo
{
	GroupInfo()
		: groups{} {}

	std::vector<JOY::Group> groups;
};



class StoredData
{

public:
	StoredData();
	~StoredData();

	bool StoreAll(const std::string& fileName);



	//Getters

	//SkeletonInfo& GetSkeletonInfo();
	LightInfo& GetLightInfo();
	CameraInfo& GetCameraInfo();
	GroupInfo& GetGroupInfo();
	std::vector<ObjectInfo>& GetObjectInfo();
	//TextureInfo& GetTextureInfo();
	MaterialInfo& GetMaterialInfo();

//	SkeletonInfo m_skeletonInfo;
	std::vector<ObjectInfo> m_objectInfoVec;
	MorphInfo m_morphInfo;

	LightInfo m_lightInfo;
	CameraInfo m_cameraInfo;
	GroupInfo m_groupInfo;
	//TextureInfo m_textureInfo;
	MaterialInfo m_materialInfo;

};

namespace JOY
{
	static StoredData data;
}