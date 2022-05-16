
#include "StoredData.h"
#include <iostream>
StoredData::StoredData()
{

}

StoredData::~StoredData()
{
}

bool StoredData::StoreAll(const std::string& fileName)
{

    std::vector<JOY::PropertyBase> propertiesInfoVec;
    JOY::PropertyBase* properties{};

    std::ifstream reader;

    reader.open(fileName, std::ios::binary);
    if (!reader.is_open())
        return false;

    Header header;
    GeneralHeader genHeader;
    

    reader.seekg(0, std::ios::end); // Set reader end.
    int totalFileByteSize = int(reader.tellg());
    reader.seekg(0, std::ios::beg); // Set reader beginning.

    while (reader.tellg() < totalFileByteSize)
    {
        reader.read((char*)&header, sizeof(Header));

        switch (header.type)
        {
        default:
            break;

        case BaseHeader::General:
        {
 

            char* ptr = (char*)&genHeader;
            ptr += sizeof(Header);

            reader.read(ptr, sizeof(GeneralHeader) - sizeof(Header));

            m_objectInfoVec.reserve(genHeader.numMeshes);

            break;
        }
        case BaseHeader::Camera:
        {

            CameraHeader cameraH;
            char* ptr = (char*)&cameraH;
            ptr += sizeof(Header);

            reader.read(ptr, sizeof(CameraHeader) - sizeof(Header));

            JOY::Camera* camPtr = new JOY::Camera[genHeader.numCameras];
            for (int i = 0; i < genHeader.numLights; i++)
            {
                reader.read((char*)camPtr, sizeof(JOY::Camera));
                m_cameraInfo.cameras.emplace_back(*camPtr);
            }
            delete[]camPtr;
            break;
        }

        case BaseHeader::Group:
        {
            GroupHeader groupH;
            char* ptr = (char*)&groupH;
            ptr += sizeof(Header);

            reader.read(ptr, sizeof(GroupHeader) - sizeof(Header));

            JOY::Group* grPtr = new JOY::Group[genHeader.numGroups];
            for (int i = 0; i < genHeader.numGroups; i++)
            {
                reader.read((char*)grPtr, sizeof(JOY::Group));
                m_groupInfo.groups.emplace_back(*grPtr);
            }
            delete[]grPtr;
            break;
        }
        case BaseHeader::Light:
        {
            LightHeader lightH;
            char* ptr = (char*)&lightH;
            ptr += sizeof(Header);

            reader.read(ptr, sizeof(LightHeader) - sizeof(Header));

            JOY::Light* liPtr = new JOY::Light[genHeader.numLights];
            for (int i = 0; i < genHeader.numLights; i++)
            {
                reader.read((char*)liPtr, sizeof(JOY::Light));
                m_lightInfo.lights.emplace_back(*liPtr);
            }
            delete[]liPtr;
            break;
        }
        case BaseHeader::Material:
        {
            //reader.ignore(header.totalByteSize); // <<<<<<<<<<<<<<<<<<<
            //break;


            MaterialHeader material;
            char* ptr = (char*)&material;
            ptr += sizeof(Header);

            reader.read(ptr, sizeof(MaterialHeader) - sizeof(Header));

            JOY::Material myMat;
            
            for (int i = 0; i < genHeader.numMaterials; i++)
            {
                reader.read((char*)&myMat, sizeof(JOY::Material));
                m_materialInfo.material.emplace_back(myMat);
            }

            break;
        }
        case BaseHeader::Object:
        {
            //reader.ignore(header.totalByteSize);  // skips entire case, remnove and make work pls thx :)
            //break;

            m_objectInfoVec.emplace_back();
            ObjectHeader& objHeader = m_objectInfoVec.back().objHeader;

            char* ptr = (char*)&objHeader;
            ptr += sizeof(Header);

            reader.read(ptr, sizeof(ObjectHeader) - sizeof(Header));

            if (objHeader.numChildren)
            {
                JOY::JoyString* childrenNames = new JOY::JoyString[objHeader.numChildren];
                reader.read((char*)childrenNames, sizeof(JOY::JoyString)* objHeader.numChildren);
                m_objectInfoVec.back().children.reserve(objHeader.numChildren);
                for (size_t i = 0; i < objHeader.numChildren; i++)
                {
                    m_objectInfoVec.back().children.emplace_back(childrenNames[i]);
                }
                delete[]childrenNames;
            }

            JOY::Vertex* verts = new JOY::Vertex[objHeader.numVerts];
            reader.read((char*)verts, sizeof(JOY::Vertex) * objHeader.numVerts);
            m_objectInfoVec.back().vertex.reserve(objHeader.numVerts);
            for (size_t i = 0; i < objHeader.numVerts; i++)
            {
                m_objectInfoVec.back().vertex.emplace_back(verts[i]);
            }
            delete[]verts;
            
            if (objHeader.hasBi)
            {
                JOY::BiNormals* biN = new JOY::BiNormals[objHeader.numVerts];
                reader.read((char*)biN, sizeof(JOY::BiNormals) * objHeader.numVerts);
                //m_objectInfoVec.back().biNormal.emplace_back(*biN);

                m_objectInfoVec.back().biNormal.reserve(objHeader.numVerts);
                for (size_t i = 0; i < objHeader.numVerts; i++)
                    m_objectInfoVec.back().biNormal.emplace_back(biN[i]);
                
                delete[]biN;
            }
            if (objHeader.skeletonIdx != -1)
            {
                JOY::WeightVTX* weights = new JOY::WeightVTX[objHeader.numVerts];
                reader.read((char*)weights, sizeof(JOY::WeightVTX) * objHeader.numVerts);
                //m_objectInfoVec.back().weights.emplace_back(*weights);

                m_objectInfoVec.back().weights.reserve(objHeader.numVerts);
                for (size_t i = 0; i < objHeader.numVerts; i++)
                    m_objectInfoVec.back().weights.emplace_back(weights[i]);

                delete[] weights;
            }

            int* index = new int[objHeader.numIdx];
            reader.read((char*)index, sizeof(int) * objHeader.numIdx);

            m_objectInfoVec.back().indices.reserve(objHeader.numIdx);
            for (size_t i = 0; i < objHeader.numIdx; i++)
            {
                m_objectInfoVec.back().indices.emplace_back(index[i]);
            }
            delete[]index;


            break;
        }
        case BaseHeader::Property:
        {
            PropertiesHeader propertyHeader;
            
            char* ptr = (char*)&propertyHeader + sizeof(Header);
            reader.read(ptr, sizeof(PropertiesHeader) - sizeof(Header));

            ObjectInfo* currentObj = nullptr;
            for (size_t i = 0; i < m_objectInfoVec.size(); i++)
            {
                if (m_objectInfoVec[i].objHeader.propertyId == propertyHeader.propertyId)
                {
                    currentObj = &m_objectInfoVec[i];
                    i = m_objectInfoVec.size();
                }
            }
            if (!currentObj)
            {
                reader.ignore(header.totalByteSize - (sizeof(SkeletonHeader) - sizeof(header))); // if object not found, skip data until next header (totalByteSize - readBytes)
                break;
            }

            currentObj->properties.reserve(propertyHeader.numProperties);

            JOY::PropertyType propertyType;
            char* type_ptr = (char*)&propertyType;
            ptr += sizeof(Header);

            std::vector<JOY::PropertyBase*> prapprapVec;


            for (int i = 0; i < propertyHeader.numProperties; i++)
            {
                reader.read(type_ptr, sizeof(JOY::PropertyType));

                switch (propertyType)
                {
                case JOY::PropertyType::NONE:
                    break;
                case JOY::PropertyType::VECTOR:
                    currentObj->properties.emplace_back(new JOY::Property<JOY::Float3>(JOY::PropertyType::VECTOR));
                    reader.read((char*)currentObj->properties.back() + sizeof(propertyType), sizeof(JOY::Property<JOY::Float3>) - sizeof(JOY::PropertyBase));
                    break;
                case JOY::PropertyType::FLOAT:
                    currentObj->properties.emplace_back(new JOY::Property<float>(JOY::PropertyType::FLOAT));
                    reader.read((char*)currentObj->properties.back() + sizeof(propertyType), sizeof(JOY::Property<float>) - sizeof(JOY::PropertyBase));
                    break;
                case JOY::PropertyType::INT:
                    currentObj->properties.emplace_back(new JOY::Property<int>(JOY::PropertyType::INT));
                    reader.read((char*)currentObj->properties.back() + sizeof(propertyType), sizeof(JOY::Property<int>) - sizeof(JOY::PropertyBase));
                    break;
                case JOY::PropertyType::BOOL:
                    currentObj->properties.emplace_back(new JOY::Property<bool>(JOY::PropertyType::BOOL));
                    reader.read((char*)currentObj->properties.back() + sizeof(propertyType), sizeof(JOY::Property<bool>) - sizeof(JOY::PropertyBase));
                    break;
                case JOY::PropertyType::STRING:
                    currentObj->properties.emplace_back(new JOY::Property<JOY::JoyString>(JOY::PropertyType::STRING));
                    reader.read((char*)currentObj->properties.back() + sizeof(propertyType), sizeof(JOY::Property<JOY::JoyString>) - sizeof(JOY::PropertyBase));
                    break;
                case JOY::PropertyType::ENUM:
                    currentObj->properties.emplace_back(new JOY::Property<int>(JOY::PropertyType::ENUM));
                    reader.read((char*)currentObj->properties.back() + sizeof(propertyType), sizeof(JOY::Property<int>) - sizeof(JOY::PropertyBase));
                    break;
                default:
                    break;
                }
            }
            break;
        }
        case BaseHeader::Skeleton:
        {
            SkeletonHeader skeleton;

            JOY::Joint jointTemp;                                            // Create object to wich the pointers will point
            JOY::KeyFrame keyFrameTemp;

            char* ptr = (char*)&skeleton;
            char* j_ptr = (char*)&jointTemp;                            // Point to the corresponding adress. Read wants a char* so probly thats why we do it like this
            char* k_ptr = (char*)&keyFrameTemp;

            ptr += sizeof(Header);                                      // Align the memory to read at correct place

            reader.read(ptr, sizeof(SkeletonHeader) - sizeof(Header));  // Read the header

            ObjectInfo* currentObj = nullptr;
            for (size_t i = 0; i < m_objectInfoVec.size(); i++)
            {
                if (m_objectInfoVec[i].objHeader.skeletonIdx == skeleton.id)
                {
                    currentObj = &m_objectInfoVec[i];
                    i = m_objectInfoVec.size();
                }
            }
            if (!currentObj)
            {
                reader.ignore(header.totalByteSize - (sizeof(SkeletonHeader) - sizeof(header))); // if object not found, skip data until next header (totalByteSize - readBytes)
                break;
            }

            currentObj->joints.reserve(skeleton.numJoints);
            for (int i = 0; i < skeleton.numJoints; i++)                // Loop through each joint, and for each joint loop through each keyframe
            {
                reader.read(j_ptr, sizeof(JOY::Joint));                      // Read all jointinfo

                currentObj->joints.emplace_back();                     // Emplace joint info
                currentObj->joints.back().jointInfo = jointTemp;
                currentObj->joints.back().keyFrames.reserve(jointTemp.numFrames);

                for (int i = 0; i < jointTemp.numFrames; i++)           // Loop through each keyframe
                {
                    reader.read(k_ptr, sizeof(JOY::KeyFrame));               // Read keyframe
                    currentObj->joints.back().keyFrames.emplace_back(keyFrameTemp); // Emplace keyframe
                }
            }

            //skeletonInfoVec.emplace_back(skeletonInfo);                 // Finnaly emplace all the info we have for ONE mesh, therefore first meshId will be 0, next 1 and so on.
            break;
        }
        case BaseHeader::Morph:
        {
           /* reader.ignore(header.totalByteSize);
            break;*/
            MorphHeader morph;
            JOY::MorphFrame mFrame;
            JOY::MorphVTX mVTX;
            JOY::MorphTargetInfo mTargetInfo;

            char* ptr = (char*)&morph;
            ptr += sizeof(Header);

            reader.read(ptr, sizeof(MorphHeader) - sizeof(Header));

            ObjectInfo* currentObj = nullptr;
            for (size_t i = 0; i < m_objectInfoVec.size(); i++)
            {
                if (m_objectInfoVec[i].objHeader.morphIdx == morph.id)
                {
                    currentObj = &m_objectInfoVec[i];
                    i = m_objectInfoVec.size();
                }
            }
            if (!currentObj)
            {
            reader.ignore(header.totalByteSize-(sizeof(MorphHeader)-sizeof(header))); // if object not found, skip data until next header (totalByteSize - readBytes)
            break;
            }

            currentObj->mInfo.reserve(morph.numDeformers);
            for (size_t i = 0; i < morph.numDeformers; i++)
            {
                currentObj->mInfo.emplace_back();

                reader.read((char*)&mTargetInfo, sizeof(JOY::MorphTargetInfo));

                currentObj->mInfo.back().morphTargetInfo = mTargetInfo;
                currentObj->mInfo.back().morphVTX.reserve(currentObj->indices.size());
                currentObj->mInfo.back().morphFrame.reserve(mTargetInfo.numFrames);


                JOY::MorphVTX* mVTX = new JOY::MorphVTX[currentObj->indices.size()];
                reader.read((char*)mVTX, sizeof(JOY::MorphVTX)* currentObj->indices.size());
                /*currentObj->mInfo.back().morphVTX.reserve(currentObj->indices.size());*/
                for (size_t i = 0; i < currentObj->indices.size(); i++)
                {
                    currentObj->mInfo.back().morphVTX.emplace_back(mVTX[i]);
                }
                delete[]mVTX;

                
                JOY::MorphFrame* mFrame = new JOY::MorphFrame[mTargetInfo.numFrames];
                reader.read((char*)mFrame, sizeof(JOY::MorphFrame)* mTargetInfo.numFrames);
                /*currentObj->mInfo.back().morphFrame.reserve(currentObj->indices.size());*/
                for (size_t i = 0; i < mTargetInfo.numFrames; i++)
                {
                    currentObj->mInfo.back().morphFrame.emplace_back(mFrame[i]);
                }
                delete[]mFrame;
                // read morph verts to mInfo.back().morphVtx (like in Object)

                //  read morph frames to mInfo.back().frames (like skeleton)

            }


            break;


        }

        }
    }
    reader.close();

    return true;

}

void StoredData::UnloadAll()
{
    for (ObjectInfo& object : m_objectInfoVec)
    {
        for (JOY::PropertyBase* property : object.properties)
            delete property;

        object.properties.clear();
        object.properties.shrink_to_fit();

        object.children.clear();
        object.children.shrink_to_fit();

        object.vertex.clear();
        object.vertex.shrink_to_fit();

        object.biNormal.clear();
        object.biNormal.shrink_to_fit();

        object.weights.clear();
        object.weights.shrink_to_fit();

        object.indices.clear();
        object.indices.shrink_to_fit();

        object.mInfo.clear();
        object.mInfo.shrink_to_fit();

        object.joints.clear();
        object.joints.shrink_to_fit();
    }
    m_objectInfoVec.clear();
    m_objectInfoVec.shrink_to_fit();

    m_lightInfo.lights.clear();
    m_lightInfo.lights.shrink_to_fit();

    m_cameraInfo.cameras.clear();
    m_cameraInfo.cameras.shrink_to_fit();

    m_groupInfo.groups.clear();
    m_groupInfo.groups.shrink_to_fit();

    m_materialInfo.material.clear();
    m_materialInfo.material.shrink_to_fit();
}

JOY::Material const * StoredData::GetMaterial(ObjectInfo& object)
{
    for (JOY::Material& material : m_materialInfo.material)
    {
        if (object.objHeader.materialName == material.matName)
            return &material;
    }
    return nullptr;
}

ObjectInfo* StoredData::GetChild(ObjectInfo& parent, size_t index)
{
    if (index >= parent.children.size())
        return nullptr;
    
    const JOY::JoyString& childName = parent.children.at(index);

    for (ObjectInfo& object : m_objectInfoVec)
    {
        if (object.objHeader.meshName == childName)
            return &object;
    }

    return nullptr;
}

//SkeletonInfo& StoredData::GetSkeletonInfo()
//{
//    return this->m_skeletonInfo;
//}

LightInfo& StoredData::GetLightInfo()
{
    return this->m_lightInfo;
}

CameraInfo& StoredData::GetCameraInfo()
{
    return this->m_cameraInfo;
}

GroupInfo& StoredData::GetGroupInfo()
{
    return this->m_groupInfo;
}

std::vector<ObjectInfo>& StoredData::GetObjectInfo()
{
    return m_objectInfoVec;
}

//TextureInfo& StoredData::GetTextureInfo()
//{
//    return this->m_textureInfo;
//}

MaterialInfo& StoredData::GetMaterialInfo()
{
    return this->m_materialInfo;
}
