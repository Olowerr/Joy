#include "Collision.h"

Collision::Collision()
{
    bBox.Center = DX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    dBox.Center = DX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    bBox.Extents = DX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    dBox.Extents = DX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    collidedY = false;
}

void Collision::collided(Object* character, Object* target, int index)
{
    stopFall = false;
    collidedY = false;
    dBox = character->GetBoundingBox(0);
    this->bBox = target->GetBoundingBox(index);
    if (dBox.Intersects(bBox))
    {
        //std::cout << "I am intersecting" << "\n";
        xIntDist = bBox.Center.x - dBox.Center.x;
        yIntDist = bBox.Center.y - dBox.Center.y;
        zIntDist = bBox.Center.z - dBox.Center.z;

        float xTemp = (bBox.Extents.x + dBox.Extents.x) - std::abs(bBox.Center.x - dBox.Center.x);
        float yTemp = (bBox.Extents.y + dBox.Extents.y) - std::abs(bBox.Center.y - dBox.Center.y);
        float zTemp = (bBox.Extents.z + dBox.Extents.z) - std::abs(bBox.Center.z - dBox.Center.z);

        if (xTemp<yTemp && xTemp < zTemp)
        {
            collidedY = false;
            stopFall = false;
            if (xIntDist < 0.0f)
            {
                distToMoveX = { (bBox.Extents.x+ dBox.Extents.x + 0.00001f)+ xIntDist, 0.0f, 0.0f };
                distToComp(distToMoveX, *character);
            }
            else
            {
                distToMoveX = { (-1.0f * (bBox.Extents.x + dBox.Extents.x + 0.00001f)) +xIntDist, 0.0f, 0.0f };
                distToComp(distToMoveX, *character);
            }
        }
        else if(yTemp < xTemp && yTemp < zTemp)
        {
            
            

            if (yIntDist < 0.0f)
            {
                collidedY = true;
                stopFall = true;
                distToMoveY = { 0.0f, (bBox.Extents.y+dBox.Extents.y) + yIntDist, 0.0f };
                distToComp(distToMoveY, *character);
            }
            else
            {
                distToMoveY = { 0.0f, (-1.0f* (bBox.Extents.y+dBox.Extents.y)) + yIntDist, 0.0f };
                distToComp(distToMoveY, *character);
            }
            
        }
        else if (zTemp < xTemp && zTemp < yTemp)
        {
            collidedY = false;
            stopFall = false;
            if (zIntDist < 0.0f)
            {
                distToMoveZ = { 0.0f, 0.0f, (bBox.Extents.z+dBox.Extents.z + 0.00001f) + zIntDist };
                distToComp(distToMoveZ, *character);
            }
            else
            {
                distToMoveZ = { 0.0f, 0.0f, (-1.0f* (bBox.Extents.z+dBox.Extents.z + 0.00001f)) + zIntDist };
                distToComp(distToMoveZ, *character);
            }
            
        }
    }
}

void Collision::distToComp(DX::XMVECTOR distToMove, Object& charBbox)
{
    charBbox.Translate(distToMove);
}

bool Collision::HitObject(Object* charBbox, Object* targetBbox, int index)
{
    dBox = charBbox->GetBoundingBox(index);
    bBox = targetBbox->GetBoundingBox(index);
    dontStopMovement = true;
    return dBox.Intersects(bBox);
    
}

bool Collision::hitItem(const DX::BoundingBox& charbBox, const DX::BoundingBox& itemBbox)
{
    return charbBox.Intersects(itemBbox);
}

bool Collision::getCollidedY()
{
    return collidedY;
}
