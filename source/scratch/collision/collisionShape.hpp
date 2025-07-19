#include "collisionPoint.hpp"
#define COLLISION_POINT_COUNT 25
#ifndef C_SHAPE_H
#define C_SHAPE_H
class CollisionShape{
    public:
        CollisionShape();
        CollisionShape(float);
        bool CheckCollision(CollisionShape);
        void SetCollision(float,float,float,float);
        CollisionPoint points[COLLISION_POINT_COUNT];
        CollisionPoint originalPoints[COLLISION_POINT_COUNT];
        float angle;
};
#endif