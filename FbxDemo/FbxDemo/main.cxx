/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

/////////////////////////////////////////////////////////////////////////
//
// This example illustrates how to detect if a scene is password 
// protected, import and browse the scene to access node and animation 
// information. It displays the content of the FBX file which name is 
// passed as program argument. You can try it with the various FBX files 
// output by the export examples.
//
/////////////////////////////////////////////////////////////////////////

#include "../Common/Common.h"
#include "DisplayCommon.h"
#include "DisplayHierarchy.h"
#include "DisplayAnimation.h"
#include "DisplayMarker.h"
#include "DisplaySkeleton.h"
#include "DisplayMesh.h"
#include "DisplayNurb.h"
#include "DisplayPatch.h"
#include "DisplayLodGroup.h"
#include "DisplayCamera.h"
#include "DisplayLight.h"
#include "DisplayGlobalSettings.h"
#include "DisplayPose.h"
#include "DisplayPivotsAndLimits.h"
#include "DisplayUserProperties.h"
#include "DisplayGenericInfo.h"

#pragma comment(lib,"libfbxsdk-mt.lib")
#pragma comment(lib,"libxml2-mt.lib")
#pragma comment(lib,"zlib-mt.lib")

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>

// Local function prototypes.
void DisplayContent(FbxScene* pScene);
void DisplayContent(FbxNode* pNode);
void DisplayTarget(FbxNode* pNode);
void DisplayTransformPropagation(FbxNode* pNode);
void DisplayGeometricTransform(FbxNode* pNode);
void DisplayMetaData(FbxScene* pScene);

static bool gVerbose = true;

int main(int argc, char** argv)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtDumpMemoryLeaks();
 
    FbxManager* lSdkManager = NULL;
    FbxScene* lScene = NULL;
    bool lResult;
    /// <summary>
    /// /
    /// </summary>
    /// <param name="argc"></param>
    /// <param name="argv"></param>
    /// <returns></returns>

    std::string fileName;
    printf("Input file: ");
    std::cin >> fileName;
    printf("\n");

    const std::string path = "../../Resources/FBXFiles/" + fileName;

    // Prepare the FBX SDK.
    InitializeSdkObjects(lSdkManager, lScene);

    if (!lSdkManager || !lScene)
        return -1;

	lResult = LoadScene(lSdkManager, lScene, path.c_str());

    if(lResult == false)
    {
        FBXSDK_printf("\n\nAn error occurred while loading the scene.");
        FBXSDK_printf("\nPress enter to close...");

        // Need both, why? Because.
        (void)getchar();
        (void)getchar();
        return -1;
    }

    else
    {
        // Display the scene.
        //DisplayMetaData(lScene);

        FBXSDK_printf("Reading Global Light Settings...\n");

        if( gVerbose ) DisplayGlobalLightSettings(&lScene->GetGlobalSettings());

        FBXSDK_printf("Reading Global Camera Settings...\n");

        if( gVerbose ) DisplayGlobalCameraSettings(&lScene->GetGlobalSettings());

        FBXSDK_printf("Reading Global Time Settings...\n");

        if( gVerbose ) DisplayGlobalTimeSettings(&lScene->GetGlobalSettings());

        FBXSDK_printf("Reading Hierarchy...\n");

        if( gVerbose ) DisplayHierarchy(lScene);

        FBXSDK_printf("Reading Content...\n");

        if( gVerbose ) DisplayContent(lScene);

        FBXSDK_printf("Reading Poses...\n");

        if( gVerbose ) DisplayPose(lScene);

        FBXSDK_printf("Reading Animations...\n");

        if( gVerbose ) DisplayAnimation(lScene);

        //now display generic information

       FBXSDK_printf("Reading Generic Information...\n");
        if( gVerbose ) DisplayGenericInfo(lScene);
    }
    

    int materialCount = lScene->GetMaterialCount();
    FbxSurfaceMaterial* material;
    for (int i = 0; i < materialCount; i++)
    {
        material = lScene->GetMaterial(i);
        if (!material)
            continue;

        FbxProperty fbxProperty = material->FindProperty(FbxLayerElement::sTextureChannelNames[0]);
        if (!fbxProperty.IsValid())
            continue;

        FbxFileTexture* lFileTexture = FbxCast<FbxFileTexture>(fbxProperty.GetSrcObject<FbxTexture>(0));

        if (lFileTexture)
            if (Writer::SetTexture(material->GetName(), lFileTexture->GetFileName()))
                Writer::CopyTextureFile(lFileTexture->GetFileName(), fileName);
        
    }

    // Destroy all objects created by the FBX SDK.
    DestroySdkObjects(lSdkManager, lResult);

    Writer::CalcNumFrames();
    fileName = fileName.substr(0, fileName.find_last_of('.')) + ".joy";
    Writer::WriteFile(fileName);

    printf("Program Finished!\n\nPress enter to close...");

    // Need both, why? Because.
    (void)getchar();
    (void)getchar();

    return 0;
}

void DisplayContent(FbxScene* pScene)
{
    int i;
    FbxNode* lNode = pScene->GetRootNode();

    if(lNode)
    {
        for(i = 0; i < lNode->GetChildCount(); i++)
        {
            DisplayContent(lNode->GetChild(i));
        }
    }
}

void DisplayContent(FbxNode* pNode)
{
    FbxNodeAttribute::EType lAttributeType;
    int i;

    if(pNode->GetNodeAttribute() == NULL)
    {
        //FBXSDK_printf("NULL Node Attribute\n\n");
    }
    else
    {
        lAttributeType = (pNode->GetNodeAttribute()->GetAttributeType());

        switch (lAttributeType)
        {
	    default:
	        break;
        case FbxNodeAttribute::eMarker:  
            //DisplayMarker(pNode);
            break;

        case FbxNodeAttribute::eSkeleton:  
            //DisplaySkeleton(pNode);
            break;

        case FbxNodeAttribute::eMesh:      
            DisplayMesh(pNode);
            break;

        case FbxNodeAttribute::eNurbs:      
            DisplayNurb(pNode);
            break;

        case FbxNodeAttribute::ePatch:     
            DisplayPatch(pNode);
            break;

        case FbxNodeAttribute::eCamera:    
            DisplayCamera(pNode);
            break;

        case FbxNodeAttribute::eLight:     
            DisplayLight(pNode);
            break;

        case FbxNodeAttribute::eLODGroup:
            DisplayLodGroup(pNode);
            break;
        }   
    }

    //DisplayUserProperties(pNode);           // here?
    //DisplayTarget(pNode);                   // here?
    //DisplayPivotsAndLimits(pNode);          // here?
    //DisplayTransformPropagation(pNode);     // here?
    //DisplayGeometricTransform(pNode);       // here?

    for(i = 0; i < pNode->GetChildCount(); i++)
    {
        DisplayContent(pNode->GetChild(i));
    }
}


void DisplayTarget(FbxNode* pNode)
{
    if(pNode->GetTarget() != NULL)
    {
        DisplayString("    Target Name: ", (char *) pNode->GetTarget()->GetName());
    }
}

void DisplayTransformPropagation(FbxNode* pNode)
{
    //FBXSDK_printf("    Transformation Propagation\n");

    // 
    // Rotation Space
    //
    EFbxRotationOrder lRotationOrder;
    pNode->GetRotationOrder(FbxNode::eSourcePivot, lRotationOrder);

    //FBXSDK_printf("        Rotation Space: ");

    switch (lRotationOrder)
    {
    case eEulerXYZ: 
        //FBXSDK_printf("Euler XYZ\n");
        break;
    case eEulerXZY:
        //FBXSDK_printf("Euler XZY\n");
        break;
    case eEulerYZX:
        //FBXSDK_printf("Euler YZX\n");
        break;
    case eEulerYXZ:
        //FBXSDK_printf("Euler YXZ\n");
        break;
    case eEulerZXY:
        //FBXSDK_printf("Euler ZXY\n");
        break;
    case eEulerZYX:
        //FBXSDK_printf("Euler ZYX\n");
        break;
    case eSphericXYZ:
        //FBXSDK_printf("Spheric XYZ\n");
        break;
    }

    //
    // Use the Rotation space only for the limits
    // (keep using eEulerXYZ for the rest)
    //
    //FBXSDK_printf("        Use the Rotation Space for Limit specification only: %s\n",
        //pNode->GetUseRotationSpaceForLimitOnly(FbxNode::eSourcePivot) ? "Yes" : "No");


    //
    // Inherit Type
    //
    FbxTransform::EInheritType lInheritType;
    pNode->GetTransformationInheritType(lInheritType);

    //FBXSDK_printf("        Transformation Inheritance: ");

    switch (lInheritType)
    {
    case FbxTransform::eInheritRrSs:
        //FBXSDK_printf("RrSs\n");
        break;
    case FbxTransform::eInheritRSrs:
        //FBXSDK_printf("RSrs\n");
        break;
    case FbxTransform::eInheritRrs:
        //FBXSDK_printf("Rrs\n");
        break;
    }
}

void DisplayGeometricTransform(FbxNode* pNode)
{
    FbxVector4 lTmpVector;

    //FBXSDK_printf("    Geometric Transformations\n");

    //
    // Translation
    //
    lTmpVector = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
    //FBXSDK_printf("        Translation: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

    //
    // Rotation
    //
    lTmpVector = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
    //FBXSDK_printf("        Rotation:    %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

    //
    // Scaling
    //
    lTmpVector = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
    //FBXSDK_printf("        Scaling:     %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);
}


void DisplayMetaData(FbxScene* pScene)
{
    return;
    FbxDocumentInfo* sceneInfo = pScene->GetSceneInfo();
    if (sceneInfo)
    {
        //FBXSDK_printf("\n\n--------------------\nMeta-Data\n--------------------\n\n");
        //FBXSDK_printf("    Title: %s\n", sceneInfo->mTitle.Buffer());
        //FBXSDK_printf("    Subject: %s\n", sceneInfo->mSubject.Buffer());
        //FBXSDK_printf("    Author: %s\n", sceneInfo->mAuthor.Buffer());
        //FBXSDK_printf("    Keywords: %s\n", sceneInfo->mKeywords.Buffer());
        //FBXSDK_printf("    Revision: %s\n", sceneInfo->mRevision.Buffer());
        //FBXSDK_printf("    Comment: %s\n", sceneInfo->mComment.Buffer());

        FbxThumbnail* thumbnail = sceneInfo->GetSceneThumbnail();
        if (thumbnail)
        {
            //FBXSDK_printf("    Thumbnail:\n");

            switch (thumbnail->GetDataFormat())
            {
            case FbxThumbnail::eRGB_24:
                //FBXSDK_printf("        Format: RGB\n");
                break;
            case FbxThumbnail::eRGBA_32:
                //FBXSDK_printf("        Format: RGBA\n");
                break;
            }

            switch (thumbnail->GetSize())
            {
	        default:
	            break;
            case FbxThumbnail::eNotSet:
                //FBXSDK_printf("        Size: no dimensions specified (%ld bytes)\n", thumbnail->GetSizeInBytes());
                break;
            case FbxThumbnail::e64x64:
                //FBXSDK_printf("        Size: 64 x 64 pixels (%ld bytes)\n", thumbnail->GetSizeInBytes());
                break;
            case FbxThumbnail::e128x128:
                //FBXSDK_printf("        Size: 128 x 128 pixels (%ld bytes)\n", thumbnail->GetSizeInBytes());
                break;
            }
        }
    }
}

