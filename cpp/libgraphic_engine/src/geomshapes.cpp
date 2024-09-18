/************************/
/*    geomshapes.cpp    */
/*    Version 2.0       */
/*     2023/04/08       */
/************************/

#include <iostream>
#include <stdexcept>
#include "geomshapes.h"
#include "mesh.h"

namespace gegt
{

    gegp::MapUVPos_FnT make_sphere(const float R, const glm::vec3& center)
    {
        return [=](const float u, const float v) -> glm::vec3 {
            const auto the     = glm::pi<float>() * u;
            const auto phi     = 2 * glm::pi<float>() * v;
            const auto r       = R * sin(the);

            const float coord1 = r * cos(phi);
            const float coord2 = R * cos(the);
            const float coord3 = r * sin(phi);
            return {coord1 + center.x, coord3 + center.y, coord2 + center.z};
        };
    }

    gegp::MapUVPos_FnT make_sphere(const float R)
    {
        const glm::vec3 center = glm::vec3(0, 0, 0);
        return make_sphere(R, center);
    }

    gegp::MapUVPos_FnT make_torus(const float r, const float R, const glm::vec3& center, const size_t mainplane)
    {
        return [=](const float u, const float v) -> glm::vec3 {
            const auto the     = 2 * glm::pi<float>() * u;
            const auto phi     = 2 * glm::pi<float>() * v;

            const float coord1 = (R + r * cos(the)) * cos(phi);
            const float coord2 = r * sin(the);
            const float coord3 = (R + r * cos(the)) * sin(phi);

            switch (mainplane)
            {
            case gegt::XY: return {coord1 + center.x, coord3 + center.y, coord2 + center.z};
            case gegt::XZ: return {coord3 + center.x, coord2 + center.y, coord1 + center.z};
            case gegt::YZ: return {coord2 + center.x, coord1 + center.y, coord3 + center.z};
            default: return {0, 0, 0};
            }
        };
    }

    gegp::MapUVPos_FnT make_torus(const float r, const float R, const size_t mainplane)
    {
        const glm::vec3 center = glm::vec3(0, 0, 0);
        return make_torus(r, R, center, mainplane);
    }

} // namespace gegt
