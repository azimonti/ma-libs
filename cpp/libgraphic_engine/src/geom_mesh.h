#ifndef _GEOM_MESH_H_43209A31A62C4B7B89C8E8E2B95022A1_
#define _GEOM_MESH_H_43209A31A62C4B7B89C8E8E2B95022A1_

/************************/
/*    geom_mesh.h       */
/*    Version 1.0       */
/*     2022/06/18       */
/************************/

#include <memory>
#include <string>
#include <vector>
#include <stddef.h>
#include "ge_mathbase.h"
#include "ge_mesh.h"
#include "geomplanes.h"

namespace ge
{
    class Mesh;
}

namespace gegt
{
    std::unique_ptr<ge::Mesh> MakeGridMeshWired(const ge::Vec2& siz, const std::array<size_t, 2>& divs,
                                                const size_t plane = XZ);
    std::unique_ptr<ge::Mesh> MakeGridMeshSolid(const ge::Vec2& siz, const std::array<size_t, 2>& divs,
                                                const size_t plane = XZ);
    std::unique_ptr<ge::Mesh> MakeCuboidMesh(bool isWired, const glm::vec3& siz, const glm::vec3& off,
                                             const ge::Color& col);
} // namespace gegt
#endif
