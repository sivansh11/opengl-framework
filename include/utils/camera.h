#ifndef CAMERA_H
#define CAMERA_H

#include "utils/bvh.h"

#include <glm/glm.hpp>

class Camera
{
public:
    Camera() = default;
    Camera(glm::vec3 &&from, glm::vec3 &&at, glm::vec3 &&up, float aspectRatio, float fov) : aspectRatio(aspectRatio), fov(fov)
    {
        set(from, at, up, aspectRatio, fov);
    }
    Camera(glm::vec3 &from, glm::vec3 &at, glm::vec3 &&up, float aspectRatio, float fov) : aspectRatio(aspectRatio), fov(fov)
    {
        set(from, at, up, aspectRatio, fov);
    }
    void set(glm::vec3 from, glm::vec3 at, glm::vec3 up, float aspectRatio, float fov)
    {
        float theta = glm::radians(fov);
        float h = glm::tan(theta / 2);
        vpHeight = 2.0f * h;
        vpWidth = aspectRatio * vpHeight;

        glm::vec3 w = glm::normalize(from - at);
        glm::vec3 u = glm::normalize(glm::cross(up, w));
        glm::vec3 v = glm::cross(w, u);

        origin = from;
        horizontal = vpWidth * u;
        vertical = vpHeight * v;
        lowerLeftCorner = origin - (horizontal / 2.f) - (vertical / 2.f) - w;
    }
    template <typename PayLoad>
    bvh::Ray<PayLoad> getRay(float u, float v)
    {
        return bvh::Ray<PayLoad>(origin, lowerLeftCorner + (u * horizontal) + (v * vertical) - origin);
    }

private:
    glm::vec3 origin;
    glm::vec3 lowerLeftCorner;
    glm::vec3 horizontal;
    glm::vec3 vertical;
    float aspectRatio;
    float fov;
    float vpHeight;
    float vpWidth;
};


#endif