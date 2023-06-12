#include "AABB.h"
#include <Novice.h>
#include "Sphere.h"


void AABB::SafeParameter(AABB& aabb)
{
    aabb.min.x = (std::min)(aabb.min.x, aabb.max.x);
    aabb.max.x = (std::max)(aabb.min.x, aabb.max.x);
    aabb.min.y = (std::min)(aabb.min.y, aabb.max.y);
    aabb.max.y = (std::max)(aabb.min.y, aabb.max.y);
    aabb.min.z = (std::min)(aabb.min.z, aabb.max.z);
    aabb.max.z = (std::max)(aabb.min.z, aabb.max.z);
}

void AABB::DrawAABB(Renderer& renderer, const AABB& aabb)
{
    Vector3 vertices[] = {
            { aabb.min.x,  aabb.max.y, aabb.min.z },
            {  aabb.max.x,  aabb.max.y, aabb.min.z },
            {  aabb.max.x, aabb.min.y, aabb.min.z },
            -aabb.min,

            { aabb.min.x,  aabb.max.y, aabb.max.z },
            aabb.max,
            {  aabb.max.x, aabb.min.y, aabb.max.z },
            { aabb.min.x, aabb.min.y, aabb.max.z }
    };

    for (uint32_t i = 0; i < 4; i++)
    {
        uint32_t j = (i + 1) % 4;
        renderer.ScreenLine(vertices[i], vertices[j], aabb.color);
        renderer.ScreenLine(vertices[i], vertices[i + 4], aabb.color);
        renderer.ScreenLine(vertices[i + 4], vertices[j + 4], aabb.color);
    }
}
