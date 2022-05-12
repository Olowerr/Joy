#pragma once
#include "JOY.h"

enum struct BaseHeader 
{ 
    General,
    Object,
    Material,
    Skeleton,
    Camera,
    Light,
    Group,
    Property,
    Morph
};

// Baseclass for all Headers
struct Header 
{
    BaseHeader type;

    // Byte width of incoming data after reading this header
    int totalByteSize;
};

// Static Data, 1 per File, always first in File
struct GeneralHeader : Header 
{
    GeneralHeader()
        :numMaterials(0), numMeshes(0), numCameras(0)
        , numLights(0), numGroups(0), animationData()
    {
        type = BaseHeader::General;
    }

    AnimationData animationData;
    int numMeshes;
    int numMaterials;
    int numCameras;
    int numLights;
    int numGroups;
};

// Dynamic Data, 1 header per Mesh
struct ObjectHeader : Header
{
    ObjectHeader()
    {
        type = BaseHeader::Object;
        numVerts = 0;
        numIdx = 0;
        hasBi = 0;
        skeletonIdx = -1;
        propertyId = -1;
        morphIdx = -1;
        numChildren = 0;
    }

    JoyString meshName;
    JoyString parentName;
    JoyString materialName;
    int numVerts;
    int numIdx;
    bool hasBi;
    int skeletonIdx;
    int morphIdx;
    int propertyId;
    int numChildren;
};

// Static data, 1 header for all Materials
struct MaterialHeader : Header 
{
	MaterialHeader()
	{
		type = BaseHeader::Material;
	}
};

// Dynamic data, 1 header per Skeleton
struct SkeletonHeader : Header
{
    SkeletonHeader()
    {
        type = BaseHeader::Skeleton;
    }

    // Shouldn't be -1 when read. If -1 something went wrong when writing
    int id = -1; 

    int numJoints = 0;
};

// Static data, 1 header for all Cameras
struct CameraHeader : Header 
{
    CameraHeader()
    {
        type = BaseHeader::Camera;
    }
};

// Static data, 1 header for all Lights
struct LightHeader : Header 
{
    LightHeader()
    {
        type = BaseHeader::Light;
    }
};

// Static data, 1 header for all Groups
struct GroupHeader : Header 
{
    GroupHeader()
    {
        type = BaseHeader::Group;
    }
};

// Dynamic data, 1 header per propertyGroup, multiple propertyGroups in file
struct PropertiesHeader : Header
{
    PropertiesHeader()
    {
        type = BaseHeader::Property;
    }
    int propertyId = -1;
    int numProperties = 0;
};

// Dynamic data, 1 header per mesh with morph animation
struct MorphHeader : Header
{
    MorphHeader()
    {
        type = BaseHeader::Morph;
    }

    int id = -1;
    int numDeformers = 0;

    /* 
    numDeformers = num of "morph animations" / blendShapes / Targets.
    Each defomers has 1 MorphTargetInfo, multiple MorphVertices & multiple MorphFrames
    Amount of MorphVertices is the same as the mesh indexCount
    Amount of MorphFrames is specified in MorphTargetInfo 

    Order of data after reading this header is:
    MorphTargetInfo
    MorphVertices (all)
    MorphFrames (all)
    Repeat based on numDeformers
    */

};