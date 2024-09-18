/************************/
/*    ge_mesh2.cpp      */
/*    Version 1.0       */
/*     2023/04/19       */
/************************/

#include <stdexcept>
#include "ge_mesh2.h"
#include "mesh.h"

namespace gegt
{
    std::unique_ptr<ge::Mesh> MakeGridMeshWired(const ge::Vec2& siz, const std::array<size_t, 2>& divs,
                                                const size_t plane)
    {
        const glm::vec4 GRID_COL = {0.6f, 0.6f, 0.6f, 1};

        std::unique_ptr<ge::Mesh> oMesh;

        switch (plane)
        {
        case XY:

        {
            const glm::vec4 GRID_COL_X0 = {0.9f, 0.3f, 0.3f, 1};
            const glm::vec4 GRID_COL_Y0 = {0.4f, 0.4f, 1.0f, 1};
            const glm::vec4 GRID_COL_Z0 = {0.4f, 1.0f, 0.4f, 1};
            oMesh                       = std::make_unique<ge::Mesh>(false);

            const auto hsiz             = siz / 2.f;

            for (size_t i = 0; i <= divs[0]; ++i)
            {
                if (i == (divs[0] / 2u)) continue;
                const float u = glm::mix(-hsiz[0], hsiz[0], static_cast<float>(i) / static_cast<float>(divs[0]));
                gegp::AddLine(oMesh->GetPosVec(), {u, -hsiz[1], 0}, {u, hsiz[1], 0});
                gegp::AddLine(oMesh->GetColVec(), GRID_COL);
            }
            for (size_t i = 0; i <= divs[1]; ++i)
            {
                if (i == (divs[1] / 2u)) continue;
                const float u = glm::mix(-hsiz[1], hsiz[1], static_cast<float>(i) / static_cast<float>(divs[1]));
                gegp::AddLine(oMesh->GetPosVec(), {-hsiz[0], u, 0}, {hsiz[0], u, 0});
                gegp::AddLine(oMesh->GetColVec(), GRID_COL);
            }

            gegp::AddLine(oMesh->GetPosVec(), {0, -hsiz[1], 0}, {0, hsiz[1], 0});
            gegp::AddLine(oMesh->GetColVec(), GRID_COL_Z0);

            gegp::AddLine(oMesh->GetPosVec(), {-hsiz[0], 0, 0}, {hsiz[0], 0, 0});
            gegp::AddLine(oMesh->GetColVec(), GRID_COL_X0);

            gegp::AddLine(oMesh->GetPosVec(), {0, 0, -siz[0] / 4}, {0, 0, siz[0] / 4});
            gegp::AddLine(oMesh->GetColVec(), GRID_COL_Y0);

            oMesh->OnGeometryUpdate();
            break;
        }

        case XZ: {

            const glm::vec4 GRID_COL_X0 = {0.9f, 0.3f, 0.3f, 1};
            const glm::vec4 GRID_COL_Y0 = {0.4f, 1.0f, 0.4f, 1};
            const glm::vec4 GRID_COL_Z0 = {0.4f, 0.4f, 1.0f, 1};
            oMesh                       = std::make_unique<ge::Mesh>(false);

            const auto hsiz             = siz / 2.f;

            for (size_t i = 0; i <= divs[0]; ++i)
            {
                if (i == (divs[0] / 2u)) continue;
                const float u = glm::mix(-hsiz[0], hsiz[0], static_cast<float>(i) / static_cast<float>(divs[0]));
                gegp::AddLine(oMesh->GetPosVec(), {u, 0, -hsiz[1]}, {u, 0, hsiz[1]});
                gegp::AddLine(oMesh->GetColVec(), GRID_COL);
            }
            for (size_t i = 0; i <= divs[1]; ++i)
            {
                if (i == (divs[1] / 2u)) continue;
                const float u = glm::mix(-hsiz[1], hsiz[1], static_cast<float>(i) / static_cast<float>(divs[1]));
                gegp::AddLine(oMesh->GetPosVec(), {-hsiz[0], 0, u}, {hsiz[0], 0, u});
                gegp::AddLine(oMesh->GetColVec(), GRID_COL);
            }

            gegp::AddLine(oMesh->GetPosVec(), {0, 0, -hsiz[1]}, {0, 0, hsiz[1]});
            gegp::AddLine(oMesh->GetColVec(), GRID_COL_Z0);

            gegp::AddLine(oMesh->GetPosVec(), {-hsiz[0], 0, 0}, {hsiz[0], 0, 0});
            gegp::AddLine(oMesh->GetColVec(), GRID_COL_X0);

            gegp::AddLine(oMesh->GetPosVec(), {0, -siz[0] / 4, 0}, {0, siz[0] / 4, 0});
            gegp::AddLine(oMesh->GetColVec(), GRID_COL_Y0);

            oMesh->OnGeometryUpdate();
            break;
        }

        case YZ: {
            const glm::vec4 GRID_COL_X0 = {0.9f, 0.3f, 0.3f, 1};
            const glm::vec4 GRID_COL_Y0 = {0.4f, 0.4f, 1.0f, 1};
            const glm::vec4 GRID_COL_Z0 = {0.4f, 1.0f, 0.4f, 1};
            oMesh                       = std::make_unique<ge::Mesh>(false);

            const auto hsiz             = siz / 2.f;

            for (size_t i = 0; i <= divs[0]; ++i)
            {
                if (i == (divs[0] / 2u)) continue;
                const float u = glm::mix(-hsiz[0], hsiz[0], static_cast<float>(i) / static_cast<float>(divs[0]));
                gegp::AddLine(oMesh->GetPosVec(), {0, u, -hsiz[1]}, {0, u, hsiz[1]});
                gegp::AddLine(oMesh->GetColVec(), GRID_COL);
            }
            for (size_t i = 0; i <= divs[1]; ++i)
            {
                if (i == (divs[1] / 2u)) continue;
                const float u = glm::mix(-hsiz[1], hsiz[1], static_cast<float>(i) / static_cast<float>(divs[1]));
                gegp::AddLine(oMesh->GetPosVec(), {0, -hsiz[0], u}, {0, hsiz[0], u});
                gegp::AddLine(oMesh->GetColVec(), GRID_COL);
            }

            gegp::AddLine(oMesh->GetPosVec(), {0, 0, -hsiz[1]}, {0, 0, hsiz[1]});
            gegp::AddLine(oMesh->GetColVec(), GRID_COL_Z0);

            gegp::AddLine(oMesh->GetPosVec(), {0, -hsiz[0], 0}, {0, hsiz[0], 0});
            gegp::AddLine(oMesh->GetColVec(), GRID_COL_X0);

            gegp::AddLine(oMesh->GetPosVec(), {-siz[0] / 4, 0, 0}, {siz[0] / 4, 0, 0});
            gegp::AddLine(oMesh->GetColVec(), GRID_COL_Y0);

            oMesh->OnGeometryUpdate();
            break;
        }
        }
        return oMesh;
    }

    std::unique_ptr<ge::Mesh> MakeGridMeshSolid(const ge::Vec2& siz, const std::array<size_t, 2>& divs,
                                                const size_t plane)
    {
        if (plane != XZ) throw std::invalid_argument("Unsupported plane");

        std::unique_ptr<ge::Mesh> oMesh;

        const auto hsiz = siz / 2.f;

        oMesh           = std::make_unique<ge::Mesh>();

        gegp::GenSolidMeshUV(*oMesh, divs[0], divs[1], nullptr, [&](float u, float v) {
            return glm::vec3(glm::mix(-hsiz[0], hsiz[0], u), 0, glm::mix(-hsiz[1], hsiz[1], v));
        }, false);

        oMesh->OnGeometryUpdate();

        return oMesh;
    }

    std::vector<glm::vec3> makeEllipseVerts(const float R1, const float R2, size_t Nu, const glm::vec3& center,
                                            const size_t plane)
    {
        std::vector<glm::vec3> verts;
        verts.reserve(Nu * 2);

        const auto dTheta = glm::two_pi<float>() / (float)Nu;
        for (size_t i = 0; i < Nu; ++i)
        {
            const auto theta = dTheta * (float)i;
            const auto x     = R1 * glm::cos(theta);
            const auto z     = R2 * glm::sin(theta);
            switch (plane)
            {
            case gegt::XY: verts.emplace_back(x, z, 0); break;
            case gegt::XZ: verts.emplace_back(x, 0, z); break;
            case gegt::YZ: verts.emplace_back(0, x, z); break;
            default: assert(false); break;
            }
        }

        for (auto& v : verts) v += center;

        return verts;
    }

    std::unique_ptr<ge::Mesh> MakeEllipseMesh(const float R1, const float R2, size_t Nu, const glm::vec3& center,
                                              const ge::Color& col, const size_t plane)
    {
        auto oMesh       = std::make_unique<ge::Mesh>();

        const auto verts = makeEllipseVerts(R1, R2, Nu, center, plane);

        auto& posV       = oMesh->GetPosVec();
        auto& colV       = oMesh->GetColVec();
        auto& norV       = oMesh->GetNorVec();

        for (size_t i = 0; i < Nu; ++i)
        {
            const auto i0  = i;
            const auto i1  = (i + 1) % Nu;
            const auto& v0 = verts[i0];
            const auto& v1 = verts[i1];
            const auto& v2 = center;
            gegp::AddTrig(posV, v0, v1, v2);
            gegp::AddTrig(colV, col, col, col);
            const auto nor = glm::normalize(glm::cross(v1 - v0, v2 - v0));
            gegp::AddTrig(norV, nor, nor, nor);
        }

        oMesh->OnGeometryUpdate();
        return oMesh;
    }

    std::unique_ptr<ge::Mesh> MakeCircleMesh(const float R, size_t Nu, const glm::vec3& center, const ge::Color& col,
                                             const size_t plane)
    {
        return MakeEllipseMesh(R, R, Nu, center, col, plane);
    }

    std::vector<glm::vec3> makeRectVerts(const float W, const float H, const glm::vec3& center, const size_t plane)
    {
        std::vector<glm::vec3> verts;
        verts.reserve(4);

        const auto hw = W * 0.5f;
        const auto hh = H * 0.5f;
        switch (plane)
        {
        case gegt::XY:
            verts.emplace_back(-hw, -hh, 0);
            verts.emplace_back(hw, -hh, 0);
            verts.emplace_back(hw, hh, 0);
            verts.emplace_back(-hw, hh, 0);
            break;
        case gegt::XZ:
            verts.emplace_back(-hw, 0, -hh);
            verts.emplace_back(hw, 0, -hh);
            verts.emplace_back(hw, 0, hh);
            verts.emplace_back(-hw, 0, hh);
            break;
        case gegt::YZ:
            verts.emplace_back(0, -hw, -hh);
            verts.emplace_back(0, hw, -hh);
            verts.emplace_back(0, hw, hh);
            verts.emplace_back(0, -hw, hh);
            break;
        default: assert(false); break;
        }

        for (auto& v : verts) v += center;

        return verts;
    }

    std::unique_ptr<ge::Mesh> MakeRectMesh(const float W, const float H, const glm::vec3& center, const ge::Color& col,
                                           const size_t plane)
    {
        auto oMesh       = std::make_unique<ge::Mesh>();

        const auto verts = makeRectVerts(W, H, center, plane);

        auto& posV       = oMesh->GetPosVec();
        auto& colV       = oMesh->GetColVec();
        auto& norV       = oMesh->GetNorVec();

        const auto nor   = glm::normalize(glm::cross(verts[1] - verts[0], verts[2] - verts[0]));
        gegp::AddTrig(posV, verts[0], verts[1], verts[2]);
        gegp::AddTrig(colV, col, col, col);
        gegp::AddTrig(norV, nor, nor, nor);
        gegp::AddTrig(posV, verts[0], verts[2], verts[3]);
        gegp::AddTrig(colV, col, col, col);
        gegp::AddTrig(norV, nor, nor, nor);

        oMesh->OnGeometryUpdate();
        return oMesh;
    }

    std::unique_ptr<ge::Mesh> MakeSquareMesh(const float W, const glm::vec3& center, const ge::Color& col,
                                             const size_t plane)
    {
        return MakeRectMesh(W, W, center, col, plane);
    }

} // namespace gegt
