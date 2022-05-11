/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include "DisplayLink.h"
#if defined (FBXSDK_ENV_MAC)
// disable the �format not a string literal and no format arguments� warning since
// the //FBXSDK_printf calls made here are all valid calls and there is no secuity risk
#pragma GCC diagnostic ignored "-Wformat-security"
#endif

void DisplayLink(FbxGeometry* pGeometry)
{
    //Display cluster now

    //int i, lLinkCount;
    //FbxCluster* lLink;

    int i, j;
    int lSkinCount=0;
    int lClusterCount=0;
    FbxCluster* lCluster;

    lSkinCount=pGeometry->GetDeformerCount(FbxDeformer::eSkin);

    const char* meshName = pGeometry->GetNode()->GetName();

    if (lSkinCount)
        Writer::CreateWeightVtx(pGeometry->GetNode());

    for(i=0; i!=lSkinCount; ++i) // I guess we can assume 1 skin per mesh?
    {
        lClusterCount = ((FbxSkin *) pGeometry->GetDeformer(i, FbxDeformer::eSkin))->GetClusterCount();
        for (j = 0; j != lClusterCount; ++j)
        {
            DisplayInt("    Cluster ", i);

            lCluster=((FbxSkin *) pGeometry->GetDeformer(i, FbxDeformer::eSkin))->GetCluster(j);

            const char* lClusterModes[] = { "Normalize", "Additive", "Total1" };

            DisplayString("    Mode: ", lClusterModes[lCluster->GetLinkMode()]);

            if(lCluster->GetLink() != NULL)
                DisplayString("        Name: ", (char *) lCluster->GetLink()->GetName());
            

            FbxString lString1 = "        Link Indices: ";
            FbxString lString2 = "        Weight Values: ";

            int k, lIndexCount = lCluster->GetControlPointIndicesCount();
            int* lIndices = lCluster->GetControlPointIndices();
            double* lWeights = lCluster->GetControlPointWeights();

            Writer::SetWeight(meshName, j, lIndexCount, lIndices, lWeights);
            Writer::AddJoint(meshName, lCluster->GetLink()->GetName());

            for(k = 0; k < lIndexCount; k++)
            {
                lString1 += lIndices[k];
                lString2 += (float) lWeights[k];


                if (k < lIndexCount - 1)
                {
                    lString1 += ", ";
                    lString2 += ", ";
                }
            }

            lString1 += "\n";
            lString2 += "\n";

            //FBXSDK_printf(lString1);
            //FBXSDK_printf(lString2);
            continue;

            DisplayString("");

            FbxAMatrix lMatrix;

            lMatrix = lCluster->GetTransformMatrix(lMatrix);
            Display3DVector("        Transform Translation: ", lMatrix.GetT());
            Display3DVector("        Transform Rotation: ", lMatrix.GetR());
            Display3DVector("        Transform Scaling: ", lMatrix.GetS());

            lMatrix = lCluster->GetTransformLinkMatrix(lMatrix);
            Display3DVector("        Transform Link Translation: ", lMatrix.GetT());
            Display3DVector("        Transform Link Rotation: ", lMatrix.GetR());
            Display3DVector("        Transform Link Scaling: ", lMatrix.GetS());

            if (lCluster->GetAssociateModel() != NULL)
            {
                lMatrix = lCluster->GetTransformAssociateModelMatrix(lMatrix);
                DisplayString("        Associate Model: ", (char *) lCluster->GetAssociateModel()->GetName());
                Display3DVector("        Associate Model Translation: ", lMatrix.GetT());
                Display3DVector("        Associate Model Rotation: ", lMatrix.GetR());
                Display3DVector("        Associate Model Scaling: ", lMatrix.GetS());
            }

            DisplayString("");
        }

        Writer::NormalizeWeights(meshName);
    }
}

