#include <citro3d.h>
#include <citro2d.h>
#include "collisionShape.hpp"
#include <iostream>
#include "image.hpp"


CollisionShape CollisionShapeFromImageData(Image::ImageRGBA image) {
    // C3D_Tex* baseTex = image->tex;
    unsigned char* data = image.data;
    // const uint32_t* pixels = (const uint32_t*)baseTex->data;
    int width = image.width;
    int height = image.height;

    // get center of visible pixels
    float sumX = 0.0f, sumY = 0.0f;
    int count = 0;

    for (int y = 50; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // uint8_t r = data[y * width * 4 + x * 4 + 0];
            // uint8_t g = data[y * width * 4 + x * 4 + 1];
            // uint8_t b = data[y * width * 4 + x * 4 + 2];
            uint8_t a = data[y * width * 4 + x * 4 + 3];

            if (a > 0) { // its not transparent so it exists
                sumX += x;
                sumY += y;
                count++;
            }            
        }
    }
    CollisionShape shape = CollisionShape(std::max(width,height)*0.25);


    if (count == 0) {
        // return the default circle if there's no shape
        return shape;
    }

    float centerX = sumX / count;
    float centerY = sumY / count;

    // find all the radii
    float radiusCircle[COLLISION_POINT_COUNT] = {0};
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // uint8_t r = data[y * width * 4 + x * 4 + 0];
            // uint8_t g = data[y * width * 4 + x * 4 + 1];
            // uint8_t b = data[y * width * 4 + x * 4 + 2];
            uint8_t a = data[y * width * 4 + x * 4 + 3];

            if (a > 0) {
                float dx = x - centerX;
                float dy = y - centerY;
                float dist = sqrt(pow(dx,2)+pow(dy,2));
                float angle = atan2(dy,dx);
                float normalizedAngle = angle;
                if (normalizedAngle < 0) normalizedAngle += 2 * M_PI;
                int index = (int)(normalizedAngle / (2.0f * M_PI) * COLLISION_POINT_COUNT);
                if (index >= COLLISION_POINT_COUNT) index = COLLISION_POINT_COUNT - 1;  // just in case
                if (dist > radiusCircle[index]) radiusCircle[index] = dist;
            }
        }
    }

    // make the shape with all the radii that were found
    for (int i = 0; i < COLLISION_POINT_COUNT; i++) {
        float angle = i * 2.0f * M_PI / COLLISION_POINT_COUNT+M_PI/2;
        shape.originalPoints[i].x = -(cos(angle) * radiusCircle[i]-centerY+width/2)*0.5;
        shape.originalPoints[i].y = (sin(angle) * radiusCircle[i])*0.5;
        shape.points[i] = shape.originalPoints[i];
    }

    return shape;
}