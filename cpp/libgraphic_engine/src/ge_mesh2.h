#ifndef _GE_MESH2_H_D5519E39BA854AC68C20FA0A8A61CAEC_
#define _GE_MESH2_H_D5519E39BA854AC68C20FA0A8A61CAEC_

/************************/
/*     ge_mesh2.h       */
/*    Version 1.0       */
/*     2023/04/19       */
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
    std::unique_ptr<ge::Mesh> MakeGridMeshWired(const ge::Vec2& siz, const std::array<size_t, 2>& divs,
                                                const size_t plane = XZ);
    std::unique_ptr<ge::Mesh> MakeGridMeshSolid(const ge::Vec2& siz, const std::array<size_t, 2>& divs,
                                                const size_t plane = XZ);
    std::unique_ptr<ge::Mesh> MakeEllipseMesh(const float R1, const float R2, size_t Nu, const glm::vec3& center,
                                              const ge::Color& col, const size_t plane = XZ);
    std::unique_ptr<ge::Mesh> MakeCircleMesh(const float R, size_t Nu, const glm::vec3& center, const ge::Color& col,
                                             const size_t plane = XZ);
    std::unique_ptr<ge::Mesh> MakeRectMesh(const float W, const float H, const glm::vec3& center, const ge::Color& col,
                                           const size_t plane = XZ);
    std::unique_ptr<ge::Mesh> MakeSquareMesh(const float W, const glm::vec3& center, const ge::Color& col,
                                             const size_t plane = XZ);

} // namespace gegt
#endif
