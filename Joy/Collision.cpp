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

        float xTemp = (bBox.Extents.x+dBox.Extents.x) - std::abs(bBox.Center.x - dBox.Center.x);
        float yTemp = (bBox.Extents.y + dBox.Extents.y) - std::abs(bBox.Center.y - dBox.Center.y);
        float zTemp = (bBox.Extents.z + dBox.Extents.z) - std::abs(bBox.Center.z - dBox.Center.z);

        if (xTemp<yTemp && xTemp < zTemp)
        {
            if (xIntDist < 0.0f)
            {
                distToMoveX = { (bBox.Extents.x+ dBox.Extents.x+0.000001f)+ xIntDist, 0.0f, 0.0f };
                distToComp(distToMoveX, *charBbox);
            }
            else
            {
                distToMoveX = { (-1.0f * (bBox.Extents.x + dBox.Extents.x)) +xIntDist, 0.0f, 0.0f };
                distToComp(distToMoveX, *charBbox);
            }
        }
        else if(yTemp < xTemp && yTemp < zTemp)
        {
            collidedY = true;
            stopFall = false;

            if (yIntDist < 0.0f)
            {
                distToMoveY = { 0.0f, (bBox.Extents.y+dBox.Extents.y) + yIntDist, 0.0f };
                distToComp(distToMoveY, *charBbox);
            }
            else
            {
                distToMoveY = { 0.0f, (-1.0f* (bBox.Extents.y+dBox.Extents.y)) + yIntDist, 0.0f };
                distToComp(distToMoveY, *charBbox);
            }
            
        }
        else if (zTemp < xTemp && zTemp < yTemp)
        {
            if (zIntDist < 0.0f)
            {
                distToMoveZ = { 0.0f, 0.0f, (bBox.Extents.z+dBox.Extents.z) + zIntDist };
                distToComp(distToMoveZ, *charBbox);
            }
            else
            {
                distToMoveZ = { 0.0f, 0.0f, (-1.0f* (bBox.Extents.z+dBox.Extents.z)) + zIntDist };
                distToComp(distToMoveZ, *charBbox);
            }
            
        }
    }
}

void Collision::distToComp(DX::XMVECTOR distToMove, Object& charBbox)
{
    charBbox.Translate(distToMove);
}

bool Collision::HitObject(Object* charBbox, Object* targetBbox)
{
    dBox = charBbox->GetBoundingBox();
    bBox = targetBbox->GetBoundingBox();
    //std::cout << dBox.Intersects(bBox) << "\n";
    stopFall = true;
    dontStopMovement = true;
    return dBox.Intersects(bBox);
    
}

bool Collision::hitItem(const DX::BoundingBox& charbBox, const DX::BoundingBox& itemBbox)
{
    return charbBox.Intersects(itemBbox);
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
