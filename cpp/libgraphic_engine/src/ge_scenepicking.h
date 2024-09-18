#ifndef _GE_SCENEPICKING_H_69A410321CA3497F9392FFD4B21DBFEF_
#define _GE_SCENEPICKING_H_69A410321CA3497F9392FFD4B21DBFEF_

/************************/
/*  ge_scenepicking.h   */
/*    Version 1.0       */
/*     2022/06/29       */
/************************/

#include <vector>
#include "ge_mathbase.h"
#include "ge_ray.h"
#include "transform.h"

namespace ge
{

    class Mesh;
    class Scene;
    class RendSetup;

    class ScenePickData
    {
      public:
        const Mesh* mpMesh{};
        size_t mTriIdx = NPOS;
        Vec3 mIntersCS{0, 0, 0};
        Vec3 mIntersWS{0, 0, 0};
        Vec2 mIntersUV{0, 0};
        float mMinLen = FLT_MAX;
        Transform mXForm_CS_WS{};
        TransformP mXForm_PS_CS{};

        bool HasPicked() const { return mTriIdx != NPOS; }

        void CalcPickedTriVertsPosCS(Vec3 out_posCS[3]) const;
    };

    class ScenePick
    {
      public:
        void BeginPicking(const RendSetup& rsetup, float x, float y, const TransformP& xf_CS_PS);

        ScenePickData EndPicking(const Mat4& xf_CS_WS);

        void AddMeshPick(const Mesh& mesh, const Mat4& xf_CS_WS);

        std::vector<const Mesh*> mpMeshes;
        Vec2 mPoint{};
        Ray mRayCS{};
    };

} // namespace ge

#endif
