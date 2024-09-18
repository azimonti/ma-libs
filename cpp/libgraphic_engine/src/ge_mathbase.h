#ifndef GE_MATHBASE_H_1C4BA0ED5A274CB3B5754F5014BF241B_
#define GE_MATHBASE_H_1C4BA0ED5A274CB3B5754F5014BF241B_

/************************/
/*    ge_mathbase.h     */
/*    Version 1.0       */
/*     2022/06/23       */
/************************/

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace ge
{
    constexpr size_t NPOS = (size_t)-1;

    using Vec2            = glm::vec2;
    using Vec3            = glm::vec3;
    using Vec4            = glm::vec4;
    using Color           = glm::vec4;
    using Mat3            = glm::mat3;
    using Mat4            = glm::mat4;

    constexpr auto MD_PI  = glm::pi<double>();
    constexpr auto MF_PI  = glm::pi<float>();

    inline float ToRad(float deg)
    {
        return deg * (MF_PI / 180);
    }

    inline double ToRad(double deg)
    {
        return deg * (MD_PI / 180);
    }

    inline auto LengthSqr = [](const auto& v) { return glm::dot(v, v); };

    constexpr Vec3 AXIS_YAW(0.f, 1.f, 0.f);
    constexpr Vec3 AXIS_PITCH(1.f, 0.f, 0.f);
} // namespace ge

#endif
