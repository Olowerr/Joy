#include "Collision.h"

Collision::Collision()
{
    bBox.Center = DX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    dBox.Center = DX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    bBox.Extents = DX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    dBox.Extents = DX::XMFLOAT3(0.0f, 0.0f, 0.0f);
}

void Collision::collided(Object* charBbox, Object* targetBbox)
{
    dontStopMovement = false;
    collidedY = false;
    dBox = charBbox->GetBoundingBox();
    bBox = targetBbox->GetBoundingBox();
    if (dBox.Intersects(bBox))
    {
        //std::cout << "I am intersecting" << "\n";
        xIntDist = bBox.Center.x - dBox.Center.x;
        yIntDist = bBox.Center.y - dBox.Center.y;
        zIntDist = bBox.Center.z - dBox.Center.z;

        if (std::abs(xIntDist) > std::abs(yIntDist) && std::abs(xIntDist) > std::abs(zIntDist))
        {
            if (xIntDist < 0.0f)
            {
                distToMoveX = { bBox.Extents.x*2.0001f + xIntDist, 0.0f, 0.0f };
                distToComp(distToMoveX, *charBbox);
            }
            else
            {
                distToMoveX = { (-1.0f * bBox.Extents.x* 2.0001f) +xIntDist, 0.0f, 0.0f };
                distToComp(distToMoveX, *charBbox);
            }
        }
        else if(std::abs(yIntDist) > std::abs(xIntDist) && std::abs(yIntDist) > std::abs(zIntDist))
        {
            collidedY = true;
            stopFall = false;

            if (yIntDist < 0.0f)
            {
                distToMoveY = { 0.0f, bBox.Extents.y* 2.f + yIntDist, 0.0f };
                distToComp(distToMoveY, *charBbox);
            }
            else
            {
                distToMoveY = { 0.0f, (-1.0f* bBox.Extents.y* 2.f) + yIntDist, 0.0f };
                distToComp(distToMoveY, *charBbox);
            }
            
        }
        else if (std::abs(zIntDist) > std::abs(xIntDist) && std::abs(zIntDist) > std::abs(yIntDist))
        {
            if (zIntDist < 0.0f)
            {
                distToMoveZ = { 0.0f, 0.0f, bBox.Extents.z* 2.0001f + zIntDist };
                distToComp(distToMoveZ, *charBbox);
            }
            else
            {
                distToMoveZ = { 0.0f, 0.0f, (-1.0f* bBox.Extents.z* 2.0001f) + zIntDist };
                distToComp(distToMoveZ, *charBbox);
            }
            
        }
    }
}

void Collision::distToComp(DX::XMVECTOR distToMove, Object& charBbox)
{
    charBbox.Translate(distToMove);
}

bool Collision::hitItem(Object* charBbox, Object* targetBbox)
{
    dBox = charBbox->GetBoundingBox();
    bBox = targetBbox->GetBoundingBox();
   // std::cout << dBox.Intersects(bBox) << "\n";
    stopFall = true;
    dontStopMovement = true;
    return dBox.Intersects(bBox);
    
}

bool Collision::GetStopFall()
{
    return stopFall;
}

bool Collision::GetDontStopMovement()
{
    return dontStopMovement;
}

bool Collision::getCollidedY()
{
    return collidedY;
}
