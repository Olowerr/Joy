#include "Collision.h"

Collision::Collision()
{
    bBox.Center = DX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    dBox.Center = DX::XMFLOAT3(0.0f, 0.9f, 0.2f);
    bBox.Extents = DX::XMFLOAT3(1.0f, 1.0f, 1.0f);
    dBox.Extents = DX::XMFLOAT3(1.0f, 1.0f, 1.0f);
    collided(dBox, bBox);
}

void Collision::collided(const DX::BoundingBox& charBbox, const DX::BoundingBox& targetBbox)
{
    

    if (charBbox.Intersects(targetBbox))
    {
        xIntDist = targetBbox.Center.x - charBbox.Center.x;
        yIntDist = targetBbox.Center.y - charBbox.Center.y;
        zIntDist = targetBbox.Center.z - charBbox.Center.z;

        if (std::abs(xIntDist) > std::abs(yIntDist) && std::abs(xIntDist) > std::abs(zIntDist))
        {
            if (xIntDist < 0.0f)
            {
                distToMoveX = { targetBbox.Extents.x + xIntDist, 0.0f, 0.0f };
                distToComp(distToMoveX);
            }
            else
            {
                distToMoveX = { (-1.0f*targetBbox.Extents.x) + xIntDist, 0.0f, 0.0f };
                distToComp(distToMoveX);
            }
            
        }
        else if(std::abs(yIntDist) > std::abs(xIntDist) && std::abs(yIntDist) > std::abs(zIntDist))
        {
            if (yIntDist < 0.0f)
            {
                distToMoveY = { 0.0f, targetBbox.Extents.y + yIntDist, 0.0f };
                distToComp(distToMoveY);
            }
            else
            {
                distToMoveY = { 0.0f, (-1.0f*targetBbox.Extents.y) + yIntDist, 0.0f };
                distToComp(distToMoveY);
            }
            
        }
        else if (std::abs(zIntDist) > std::abs(xIntDist) && std::abs(zIntDist) > std::abs(yIntDist))
        {
            if (zIntDist < 0.0f)
            {
                distToMoveZ = { 0.0f, 0.0f, targetBbox.Extents.z + zIntDist };
                distToComp(distToMoveZ);
            }
            else
            {
                distToMoveZ = { 0.0f, 0.0f, (-1.0f*targetBbox.Extents.z) + zIntDist };
                distToComp(distToMoveZ);
            }
            
        }
    }
}

void Collision::distToComp(DX::XMVECTOR distToMove)
{
    dBox.Center.x += distToMove.m128_f32[0];
    dBox.Center.y += distToMove.m128_f32[1];
    dBox.Center.z += distToMove.m128_f32[2];

}