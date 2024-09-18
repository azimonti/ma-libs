/************************/
/*   ge_mesh3.cpp       */
/*    Version 1.0       */
/*     2022/06/19       */
/************************/

#include <stdexcept>
#include "ge_mesh3.h"
#include "mesh.h"

namespace gegt
{

    std::unique_ptr<ge::Mesh> MakeCuboidMesh(bool isWired, const glm::vec3& siz, const glm::vec3& off,
                                             const ge::Color& col)
    {
        auto oMesh       = std::make_unique<ge::Mesh>(!isWired);

        const auto verts = gegp::MakeCubeVerts(-siz * 0.5f + off, siz * 0.5f + off);

        auto& posV       = oMesh->GetPosVec();
        auto& colV       = oMesh->GetColVec();

        if (isWired)
        {
            // bottom
            gegp::AddLine(posV, verts[0], verts[1]);
            gegp::AddLine(colV, col);
            gegp::AddLine(posV, verts[1], verts[5]);
            gegp::AddLine(colV, col);
            gegp::AddLine(posV, verts[5], verts[4]);
            gegp::AddLine(colV, col);
            gegp::AddLine(posV, verts[4], verts[0]);
            gegp::AddLine(colV, col);
            // top
            gegp::AddLine(posV, verts[2], verts[3]);
            gegp::AddLine(colV, col);
            gegp::AddLine(posV, verts[3], verts[7]);
            gegp::AddLine(colV, col);
            gegp::AddLine(posV, verts[7], verts[6]);
            gegp::AddLine(colV, col);
            gegp::AddLine(posV, verts[6], verts[2]);
            gegp::AddLine(colV, col);
            // vertical
            gegp::AddLine(posV, verts[3], verts[1]);
            gegp::AddLine(colV, col);
            gegp::AddLine(posV, verts[7], verts[5]);
            gegp::AddLine(colV, col);
            gegp::AddLine(posV, verts[6], verts[4]);
            gegp::AddLine(colV, col);
            gegp::AddLine(posV, verts[2], verts[0]);
            gegp::AddLine(colV, col);
        }
        else
        {
            static const glm::vec3 norFaces[6] = {
                glm::vec3(1, 0, 0),  // px
                glm::vec3(-1, 0, 0), // nx
                glm::vec3(0, 1, 0),  // py
                glm::vec3(0, -1, 0), // ny
                glm::vec3(0, 0, 1),  // pz
                glm::vec3(0, 0, -1), // nz
            };

            static const int posFaces[6][4] = {
                {7, 5, 4, 6},
                {1, 3, 2, 0},
                {3, 7, 6, 2},
                {0, 4, 5, 1},
                {3, 1, 5, 7},
                {0, 2, 6, 4},
            };

            auto& norV = oMesh->GetNorVec();
            for (size_t i = 0; i != 6; ++i)
            {
                const auto& v0 = verts[(size_t)posFaces[i][0]];
                const auto& v1 = verts[(size_t)posFaces[i][1]];
                const auto& v2 = verts[(size_t)posFaces[i][2]];
                const auto& v3 = verts[(size_t)posFaces[i][3]];
                gegp::AddQuad(posV, v0, v1, v3, v2);
                gegp::AddQuad(colV, col, col, col, col);
                const auto nor = norFaces[i];
                gegp::AddQuad(norV, nor, nor, nor, nor);
            }
            // oMesh->GetNorVec() = gegp::BuildVertexNormalsFlat( posV );
        }

        oMesh->OnGeometryUpdate();

        return oMesh;
    }

    std::vector<glm::vec3> makeCylinderVerts(const float R, const float H, size_t Nu, const glm::vec3& center,
                                             const size_t plane)
    {
        std::vector<glm::vec3> verts;
        verts.reserve(Nu * 2);

        const auto dTheta = glm::two_pi<float>() / (float)Nu;
        for (size_t i = 0; i < Nu; ++i)
        {
            const auto theta = dTheta * (float)i;
            const auto x     = R * glm::cos(theta);
            const auto z     = R * glm::sin(theta);
            switch (plane)
            {
            case gegt::XY:
                verts.emplace_back(x, z, -H * 0.5f);
                verts.emplace_back(x, z, H * 0.5f);
                break;
            case gegt::XZ:
                verts.emplace_back(x, -H * 0.5f, z);
                verts.emplace_back(x, H * 0.5f, z);
                break;
            case gegt::YZ:
                verts.emplace_back(-H * 0.5f, x, z);
                verts.emplace_back(H * 0.5f, x, z);
                break;
            default: assert(false); break;
            }
        }

        for (auto& v : verts) v += center;

        return verts;
    }

    std::unique_ptr<ge::Mesh> MakeCylinderMesh(const float R, const float H, size_t Nu, const glm::vec3& center,
                                               const ge::Color& col, const size_t plane)
    {
        auto oMesh       = std::make_unique<ge::Mesh>();

        const auto verts = makeCylinderVerts(R, H, Nu, center, plane);

        auto& posV       = oMesh->GetPosVec();
        auto& colV       = oMesh->GetColVec();
        auto& norV       = oMesh->GetNorVec();

        for (size_t i = 0; i < Nu; ++i)
        {
            const auto i0  = i * 2;
            const auto i1  = (i + 1) % Nu * 2;
            const auto& v0 = verts[i0];
            const auto& v1 = verts[i1];
            const auto& v2 = verts[i0 + 1];
            const auto& v3 = verts[i1 + 1];
            gegp::AddQuad(posV, v0, v1, v2, v3);
            gegp::AddQuad(colV, col, col, col, col);
            const auto nor = glm::normalize(glm::cross(v1 - v0, v2 - v0));
            gegp::AddQuad(norV, nor, nor, nor, nor);
        }

        glm::vec3 capNor(0, 1, 0), topCenter(0, 0, 0), botCenter(0, 0, 0);
        switch (plane)
        {
        case gegt::XY:
            capNor    = glm::vec3(0, 0, 1);
            topCenter = center + glm::vec3(0, 0, H * 0.5f);
            botCenter = center + glm::vec3(0, 0, -H * 0.5f);
            break;
        case gegt::XZ:
            capNor    = glm::vec3(0, 1, 0);
            topCenter = center + glm::vec3(0, H * 0.5f, 0);
            botCenter = center + glm::vec3(0, -H * 0.5f, 0);
            break;
        case gegt::YZ:
            capNor    = glm::vec3(1, 0, 0);
            topCenter = center + glm::vec3(H * 0.5f, 0, 0);
            botCenter = center + glm::vec3(-H * 0.5f, 0, 0);
            break;
        }

        // add the caps
        for (size_t i = 0; i < Nu; ++i)
        {
            const auto i0     = i * 2;
            const auto i1     = (i + 1) % Nu * 2;

            const auto& topV0 = verts[i0 + 1];
            const auto& topV1 = verts[i1 + 1];
            const auto& botV0 = verts[i0];
            const auto& botV1 = verts[i1];

            gegp::AddTrig(posV, topCenter, topV0, topV1);
            gegp::AddTrig(colV, col, col, col);
            gegp::AddTrig(norV, capNor, capNor, capNor);
            gegp::AddTrig(posV, botCenter, botV0, botV1);
            gegp::AddTrig(colV, col, col, col);
            gegp::AddTrig(norV, -capNor, -capNor, -capNor);
        }
        oMesh->OnGeometryUpdate();
        return oMesh;
    }

} // namespace gegt
