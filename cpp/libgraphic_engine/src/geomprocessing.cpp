/************************/
/* geomprocessing.cpp   */
/*    Version 1.0       */
/*     2022/06/15       */
/************************/

#include "geomprocessing.h"
#include "mesh.h"

#define USE_INDEX_MESH

namespace gegp
{

    std::vector<glm::vec3> BuildVertexNormalsFlat(const std::vector<glm::vec3>& posV)
    {
        assert((posV.size() % 3) == 0); // must be triangles

        std::vector<glm::vec3> norV(posV.size(), glm::vec3(0, 0, 0));
        // sum contributions
        for (size_t i = 0; i < posV.size(); i += 3)
        {
            const auto faceNor = glm::normalize(glm::triangleNormal(posV[i + 1], posV[i + 0], posV[i + 2]));
            norV[i + 0]        = faceNor;
            norV[i + 1]        = faceNor;
            norV[i + 2]        = faceNor;
        }

        return norV;
    }

    // generates a generic wired-mesh for a UV-mapped surface
    void GenWiredMeshUV(ge::Mesh& mesh, size_t Nu, size_t Nv, const glm::vec4& col, const MapUVPos_FnT& mapUV)
    {
        const auto norV = 1.f / (float)Nv;
        const auto norU = 1.f / (float)Nu;
        for (size_t Iv = 0; Iv <= Nv; ++Iv)
        {
            const auto v0 = (float)(Iv + 0) * norV;
            const auto v1 = (float)(Iv + 1) * norV;
            for (size_t Iu = 0; Iu <= Nu; ++Iu)
            {
                const auto u0 = (float)(Iu + 0) * norU;
                const auto u1 = (float)(Iu + 1) * norU;
                if (Iu != Nu)
                {
                    AddLine(mesh.GetPosVec(), mapUV(u0, v0), mapUV(u1, v0));
                    AddLine(mesh.GetColVec(), col);
                }
                if (Iv != Nv)
                {
                    AddLine(mesh.GetPosVec(), mapUV(u0, v0), mapUV(u0, v1));
                    AddLine(mesh.GetColVec(), col);
                }
            }
        }
    }

    // generates a generic wired-mesh for a UV-mapped surface
    void GenSolidMeshUV(ge::Mesh& mesh, size_t Nu, size_t Nv, const MapUVCol_FnT& mapUVCol, const MapUVPos_FnT& mapUV,
                        bool isManifold)
    {
#ifdef USE_INDEX_MESH
        // build the vertices
        const size_t genVertsVN = isManifold ? Nv : Nv + 1;
        const size_t genVertsUN = isManifold ? Nu : Nu + 1;
        for (size_t Iv = 0; Iv < genVertsVN; ++Iv)
        {
            const auto v0 = (float)(Iv + 0) / (float)Nv;
            for (size_t Iu = 0; Iu < genVertsUN; ++Iu)
            {
                const auto u0 = (float)(Iu + 0) / (float)Nu;
                mesh.GetPosVec().push_back(mapUV(u0, v0));
                if (mapUVCol) mesh.GetColVec().push_back(mapUVCol(u0, v0));
            }
        }

        // build the indices as a grid (NOTE: verts at edges of the grid are shared to avoid seams)
        for (size_t Iv = 0; Iv < Nv; ++Iv)
        {
            const auto rowV0 = (uint32_t)(genVertsUN * Iv);
            const auto rowV1 = (uint32_t)(genVertsUN * (isManifold && Iv == (Nv - 1) ? 0 : Iv + 1));
            for (size_t Iu = 0; Iu < Nu; ++Iu)
            {
                const auto U0 = (uint32_t)(Iu);
                const auto U1 = (uint32_t)(isManifold && Iu == (Nu - 1) ? 0 : Iu + 1);
                AddQuad(mesh.GetIdxVec(), rowV0 + U0, rowV0 + U1, rowV1 + U0, rowV1 + U1);
            }
        }

        // calculate the normals at the vertices
        mesh.GetNorVec() = BuildVertexNormals(mesh.GetPosVec(), mesh.GetIdxVec());
#else
        const auto norV = 1.f / Nv;
        const auto norU = 1.f / Nu;

        auto calcNorm   = [&](auto Iu, auto Iv) {
            glm::vec3 norSum{};
            for (size_t vSubI = 0; vSubI < 2; ++vSubI)
            {
                for (size_t uSubI = 0; uSubI < 2; ++uSubI)
                {
                    const auto v0    = (float)(vSubI + Iv + 0 - 1) * norV;
                    const auto v1    = (float)(vSubI + Iv + 1 - 1) * norV;
                    const auto u0    = (float)(uSubI + Iu + 0 - 1) * norU;
                    const auto u1    = (float)(uSubI + Iu + 1 - 1) * norU;
                    const auto pos00 = mapUV(u0, v0);
                    const auto pos01 = mapUV(u0, v1);
                    const auto pos10 = mapUV(u1, v0);
                    norSum += glm::triangleNormal(pos10, pos00, pos01);
                }
            }
            return glm::normalize(norSum);
        };

        for (size_t Iv = 0; Iv < Nv; ++Iv)
        {
            const auto v0 = (float)(Iv + 0) * norV;
            const auto v1 = (float)(Iv + 1) * norV;
            for (size_t Iu = 0; Iu < Nu; ++Iu)
            {
                const auto u0    = (float)(Iu + 0) * norU;
                const auto u1    = (float)(Iu + 1) * norU;
                const auto pos00 = mapUV(u0, v0);
                const auto pos01 = mapUV(u0, v1);
                const auto pos10 = mapUV(u1, v0);
                const auto pos11 = mapUV(u1, v1);

                const auto nor00 = calcNorm(Iu + 0, Iv + 0);
                const auto nor01 = calcNorm(Iu + 0, Iv + 1);
                const auto nor10 = calcNorm(Iu + 1, Iv + 0);
                const auto nor11 = calcNorm(Iu + 1, Iv + 1);

                AddQuad(mesh.GetPosVec(), pos00, pos10, pos01, pos11);
                if (mapUVCol) AddQuad(mesh.GetColVec(), mapUVCol(u0, v0));
                AddQuad(mesh.GetNorVec(), nor00, nor10, nor01, nor11);
            }
        }
#endif
    }

    // generates a generic wired-mesh for a UV-mapped surface
    void GenMeshUV(ge::Mesh& mesh, size_t Nu, size_t Nv, const glm::vec4& col, const MapUVPos_FnT& mapUV,
                   bool isManifold)
    {
        if (mesh.IsTriangles()) GenSolidMeshUV(mesh, Nu, Nv, [&](auto, auto) { return col; }, mapUV, isManifold);
        else GenWiredMeshUV(mesh, Nu, Nv, col, mapUV);
    }

} // namespace gegp
