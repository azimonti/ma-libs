/************************/
/* ge_scenepicking.cpp  */
/*    Version 1.0       */
/*     2022/06/29       */
/************************/

#include "ge_geomutils.h"
#include "ge_scenepicking.h"
#include "mesh.h"
#include "scene.h"

namespace ge
{

    void ScenePickData::CalcPickedTriVertsPosCS(Vec3 out_posCS[3]) const
    {
        if (!mpMesh || !HasPicked())
        {
            out_posCS = {};
            (void)out_posCS;
            return;
        }

        const auto& posV = mpMesh->GetPosVec();
#if 0
    const auto &idxV = mpMesh->GetIdxVec();
    size_t i0 {}; (void)i0;
    size_t i1 {}; (void)i1;
    size_t i2 {}; (void)i2;

    if ( idxV.empty() )
    {
        i0 = mTriIdx*3+0;
        i1 = mTriIdx*3+1;
        i2 = mTriIdx*3+2;
    }
    else
    {
        i0 = idxV[ mTriIdx*3+0 ];
        i1 = idxV[ mTriIdx*3+1 ];
        i2 = idxV[ mTriIdx*3+2 ];
    }
#endif
        const auto p0       = posV[mTriIdx * 3 + 0];
        const auto p1       = posV[mTriIdx * 3 + 1];
        const auto p2       = posV[mTriIdx * 3 + 2];

        const auto xf_CS_OS = mXForm_CS_WS * mpMesh->GetTransform();
        out_posCS[0]        = xf_CS_OS.XFormPos(p0);
        out_posCS[1]        = xf_CS_OS.XFormPos(p1);
        out_posCS[2]        = xf_CS_OS.XFormPos(p2);
    }

    void ScenePick::BeginPicking(const RendSetup& rsetup, float x, float y, const TransformP& xf_CS_PS)
    {
        const auto drc    = Vec4{0.f, 0.f, (float)rsetup.mRTargetSiz[0], (float)rsetup.mRTargetSiz[1]};

        const auto pickNS = Vec2(-1 + 2 * (x - drc[0]) / drc[2], 1 - 2 * (y - drc[1]) / drc[3]);

        // mPoint = pickNS;
        // printf( "Picking %f %f\n", pickNS[0], pickNS[1] );

        // calc the ray in camera-space
        mRayCS            = CalcRayCS_NS(xf_CS_PS, pickNS);
#if 0
    printf( "Picking Ray. Pos:%f %f %f Dir:%f %f %f\n",
        mRayCS.mSta[0], mRayCS.mSta[1], mRayCS.mSta[1],
        mRayCS.mDir[0], mRayCS.mDir[1], mRayCS.mDir[1] );
#endif
        mpMeshes.clear();
    }

    // http://geomalgorithms.com/a06-_intersect-2.html#intersect3D_RayTriangle()
    template <bool IS_SEGMENT>
    inline bool CalcRaySegTriIntersectionAndUV(const Vec3& r0, const Vec3& r1, const Vec3& v0, const Vec3& v1,
                                               const Vec3& v2, const Vec3& n, Vec3* out_pInters = nullptr,
                                               Vec2* out_pUVs = nullptr, float* io_minLen = nullptr)
    {
        // ray direction vector
        const auto dir  = (IS_SEGMENT ? r1 - r0 : r1);
        const auto w0   = r0 - v0;

        const auto a    = -glm::dot(n, w0);
        const auto b    = glm::dot(n, dir);

        // use a very small epsilon because we may be dealing with
        // pretty small source values, since this testing doesn't require
        // normals to be unitary...
        const float EPS = 1e-30f;
        if (fabs(b) < EPS) // ray is  parallel to triangle plane
        {
            // if (a == 0)                 // ray lies in triangle plane
            //     return 2;
            // else
            return false; // ray disjoint from plane
        }

        // get intersect point of ray with triangle plane
        const auto r = a / b;

        if (r < 0 || (IS_SEGMENT && r > 1)) return false;

        if (io_minLen && r > *io_minLen) return false;

        const auto inters     = r0 + r * dir; // intersect point of ray and plane

        // get triangle edge vectors
        const auto u          = v1 - v0;
        const auto v          = v2 - v0;

        // is inters inside T?
        const auto uu         = glm::dot(u, u);
        const auto uv         = glm::dot(u, v);
        const auto vv         = glm::dot(v, v);
        const auto w          = inters - v0;
        const auto wu         = glm::dot(w, u);
        const auto wv         = glm::dot(w, v);
        const auto D          = uv * uv - uu * vv;

        const auto ooD        = 1.f / D;

        static float EPS_EDGE = 1e-5f;

        // get and test parametric coords
        const auto s          = (uv * wv - vv * wu) * ooD;
        if (s < (-EPS_EDGE) || s > (1 + EPS_EDGE)) return false; // inters is outside T

        const auto t = (uv * wu - uu * wv) * ooD;
        if (t < (-EPS_EDGE) || (s + t) > (1 + EPS_EDGE)) return false; // inters is outside T

        if (out_pInters) *out_pInters = inters;

        if (out_pUVs) *out_pUVs = {s, t};

        if (io_minLen) *io_minLen = r;

        return true; // inters is in T
    }

    ScenePickData ScenePick::EndPicking(const Mat4& xf_CS_WS)
    {
        ScenePickData data;

        for (const auto* pMesh : mpMeshes)
        {
            if (!pMesh->IsTriangles()) continue;

            const auto& xf_WS_OS           = pMesh->GetTransform();
            const auto xf_CS_OS            = xf_CS_WS * xf_WS_OS;
            const auto xf_OS_CS            = glm::inverse(xf_CS_OS);
            // object-space rays
            const auto rayPosOS            = XFormPos(xf_OS_CS, mRayCS.mSta);
            const auto rayDirOS            = XFormNor(xf_OS_CS, mRayCS.mDir);

            const auto& idxV               = pMesh->GetIdxVec();
            const auto& posV               = pMesh->GetPosVec();

            const size_t primVertsN        = !idxV.empty() ? idxV.size() : posV.size();

            const uint32_t* const pIndices = !idxV.empty() ? idxV.data() : nullptr;

            assert((primVertsN % 3) == 0);
            for (size_t i = 0, trig = 0; i != primVertsN; i += 3, trig += 1)
            {
                Vec3 v0, v1, v2;
                if (pIndices)
                {
                    v0 = posV[pIndices[i + 0]];
                    v1 = posV[pIndices[i + 1]];
                    v2 = posV[pIndices[i + 2]];
                }
                else
                {
                    v0 = posV[i + 0];
                    v1 = posV[i + 1];
                    v2 = posV[i + 2];
                }

                // auto triNorOS = DE3::CalcNormal( v0, v1, v2 );
                const auto triNorOS = glm::normalize(glm::triangleNormal(v1, v0, v2));

                Vec3 intersPosOS;
                if (CalcRaySegTriIntersectionAndUV<false>(rayPosOS, rayDirOS, v0, v1, v2, triNorOS, &intersPosOS,
                                                          &data.mIntersUV, &data.mMinLen))
                {
                    data.mpMesh    = pMesh;
                    data.mTriIdx   = trig;

                    data.mIntersWS = XFormPos(xf_WS_OS, intersPosOS);
                    data.mIntersCS = XFormPos(xf_CS_OS, intersPosOS);
                }
            }
        }

#if 0
    printf( "Intersected %zu meshes\n", mpPick_Meshes.size() );

    if ( data.HasPicked() )
        printf( "Intersected at PosCS:%f %f %f\n",
            data.mIntersCS[0], data.mIntersCS[1], data.mIntersCS[2] );
#endif
        mpMeshes.clear();

        return data;
    }

    void ScenePick::AddMeshPick(const Mesh& mesh, const Mat4& xf_CS_WS)
    {
        const auto& xf_WS_OS = mesh.GetTransform();

        const auto intersN   = RaySphereIntersectionsFromBBox(mRayCS, mesh.GetBBox(), xf_CS_WS * xf_WS_OS);

        if (intersN) mpMeshes.push_back(&mesh);
    }

} // namespace ge
