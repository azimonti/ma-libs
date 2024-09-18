#ifndef _GEOM_MESH3_H_43209A31A62C4B7B89C8E8E2B95022A1_
#define _GEOM_MESH3_H_43209A31A62C4B7B89C8E8E2B95022A1_

/************************/
/*    ge_mesh3.h        */
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

namespace ge
{
    class Mesh;
}

namespace gegt
{

    std::unique_ptr<ge::Mesh> MakeCuboidMesh(bool isWired, const glm::vec3& siz, const glm::vec3& off,
                                             const ge::Color& col);
    std::unique_ptr<ge::Mesh> MakeCylinderMesh(const float R, const float H, size_t Nu, const glm::vec3& center,
                                               const ge::Color& col, const size_t plane = XZ);

} // namespace gegt
#endif
