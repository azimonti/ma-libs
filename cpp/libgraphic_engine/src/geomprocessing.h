#ifndef _GEOMPROCESSING_H_439E048C92F4406980F1205FFF1BA323_
#define _GEOMPROCESSING_H_439E048C92F4406980F1205FFF1BA323_

/************************/
/* geomprocessing.h     */
/*    Version 1.0       */
/*     2022/06/15       */
/************************/

#include <array>
#include <functional>
#include <vector>
#include <assert.h>
#include "ge_mathbase.h"

namespace ge
{
    class Mesh;
}

namespace gegp
{
    using MapUVPos_FnT = std::function<glm::vec3(float, float)>;
    using MapUVCol_FnT = std::function<glm::vec4(float, float)>;

    template <typename T> inline void AddLine(std::vector<T>& vec, const T& v0, const T& v1)
    {
        vec.push_back(v0);
        vec.push_back(v1);
    }

    template <typename T> inline void AddTrig(std::vector<T>& vec, const T& v0, const T& v1, const T& v2)
    {
        vec.push_back(v0);
        vec.push_back(v1);
        vec.push_back(v2);
    }

    template <typename T> inline void AddQuad(std::vector<T>& vec, const T& v0, const T& v1, const T& v2, const T& v3)
    {
        vec.push_back(v0);
        vec.push_back(v1);
        vec.push_back(v2);
        vec.push_back(v2);
        vec.push_back(v1);
        vec.push_back(v3);
    }

    template <typename T> inline void FillVerts(std::vector<T>& vec, size_t n, const T& v0)
    {
        for (size_t i = 0; i < n; ++i) vec.push_back(v0);
    }

    template <typename T> inline void AddLine(std::vector<T>& vec, const T& v0)
    {
        FillVerts(vec, 2, v0);
    }

    template <typename T> inline void AddTrig(std::vector<T>& vec, const T& v0)
    {
        FillVerts(vec, 3, v0);
    }

    template <typename T> inline void AddQuad(std::vector<T>& vec, const T& v0)
    {
        FillVerts(vec, 6, v0);
    }

    std::vector<glm::vec3> BuildVertexNormalsFlat(const std::vector<glm::vec3>& posV);

    template <typename IT>
    inline std::vector<glm::vec3> BuildVertexNormals(const std::vector<glm::vec3>& posV, const std::vector<IT>& idxV)
    {
        assert((idxV.size() % 3) == 0); // must be triangles

        std::vector<glm::vec3> norV(posV.size(), glm::vec3(0, 0, 0));
        // sum contributions
        for (size_t i = 0; i < idxV.size(); i += 3)
        {
            const IT vi0       = idxV[i + 0];
            const IT vi1       = idxV[i + 1];
            const IT vi2       = idxV[i + 2];

            const auto faceNor = glm::triangleNormal(posV[vi1], posV[vi0], posV[vi2]);
            norV[vi0] += faceNor;
            norV[vi1] += faceNor;
            norV[vi2] += faceNor;
        }
        // normalize
        for (auto& n : norV) n = glm::normalize(n);

        return norV;
    }

    void GenWiredMeshUV(ge::Mesh& mesh, size_t uN, size_t vN, const glm::vec4& col, const MapUVPos_FnT& mapUV);
    void GenSolidMeshUV(ge::Mesh& mesh, size_t uN, size_t vN, const MapUVCol_FnT& mapUVCol, const MapUVPos_FnT& mapUV,
                        bool isManifold);
    void GenMeshUV(ge::Mesh& mesh, size_t uN, size_t vN, const glm::vec4& col, const MapUVPos_FnT& mapUV,
                   bool isManifold);

    template <typename T> inline std::array<T, 8> MakeCubeVerts(const T& mi, const T& ma)
    {
        /*
                y          2 ----- 6           __________
                |__ x    / |     / |          /|        /|
               /        3 -+--- 7  |         /_|______ / |
              z         |  0 ---+- 4        |  |______|__|
                        |/      |/          | /       | /
                        1 ----- 5           |/________|/
        */
        return std::array<T, 8>{
            mi,
            {mi[0], mi[1], ma[2]},
            {mi[0], ma[1], mi[2]},
            {mi[0], ma[1], ma[2]},
            {ma[0], mi[1], mi[2]},
            {ma[0], mi[1], ma[2]},
            {ma[0], ma[1], mi[2]},
            ma
        };
    }
} // namespace gegp
#endif
