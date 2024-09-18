#ifndef _GEOMSHAPES_H_43209A31A62C4B7B89C8E8E2B95022A1_
#define _GEOMSHAPES_H_43209A31A62C4B7B89C8E8E2B95022A1_

/************************/
/*     geomshapes.h     */
/*    Version 1.0       */
/*     2022/06/18       */
/************************/

#include <memory>
#include <string>
#include <vector>
#include <stddef.h>
#include "ge_mathbase.h"
#include "geomplanes.h"
#include "geomprocessing.h"

namespace gegt
{

    gegp::MapUVPos_FnT make_sphere(const float R, const glm::vec3& center);
    gegp::MapUVPos_FnT make_sphere(const float R);
    gegp::MapUVPos_FnT make_torus(const float r, const float R, const glm::vec3& center,
                                  const size_t mainplane = gegt::XY);
    gegp::MapUVPos_FnT make_torus(const float r, const float R, const size_t mainplane = gegt::XY);

} // namespace gegt
#endif
