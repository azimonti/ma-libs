#ifndef _GE_GEOMUTILS_H_98A2327AEF1043C38F40F921924F800B_
#define _GE_GEOMUTILS_H_98A2327AEF1043C38F40F921924F800B_

/************************/
/*    ge_geomutils.h    */
/*    Version 1.0       */
/*     2022/06/29       */
/************************/

#include "ge_bbox.h"
#include "ge_ray.h"

namespace ge
{

    inline unsigned RaySphereIntersections(const Ray& ray, const Vec3& center, float radiusSqr)

    {
        // solve the quadratic equation
        const auto a = LengthSqr(ray.mDir);
        if (!a) return 0;

        const auto b    = 2.0f * (glm::dot(ray.mSta, ray.mDir) - glm::dot(ray.mDir, center));
        const auto c    = LengthSqr(center - ray.mSta) - radiusSqr;
        const auto disc = b * b - 4 * a * c;

        return disc < 0.f ? 0u : (disc == 0 ? 1u : 2u);
    }

    inline unsigned RaySphereIntersectionsFromBBox(const Ray& ray, const BBox& bbox, const Transform& mtxRay_BB)
    {
        const auto bbRayMin = mtxRay_BB.XFormPos(bbox.mMin);
        const auto bbRayMax = mtxRay_BB.XFormPos(bbox.mMax);

        const auto center   = (bbRayMin + bbRayMax) * 0.5f;
        const auto radSqr   = LengthSqr(bbRayMax - center);

        return RaySphereIntersections(ray, center, radSqr);
    }

    inline Ray CalcRayCS_NS(const TransformP& xf_CS_PS, const Vec2& posNS)
    {
        auto outRayCS = Ray(true);

        // float FAR_Z  =  0.9f; // not 1 because of inf-projection 1-eps
#ifdef GE_HAS_NEAR_CLIP_AT_0
        float NEAR_Z = 0.1f;
#else
        float NEAR_Z = -0.9f;
#endif
        float FAR_Z   = 0.9f;

        // if ( farLessThanNear )
        //     std::swap( NEAR_Z, FAR_Z );

        auto posCS4_n = XFormPos4(xf_CS_PS, Vec3(posNS[0], posNS[1], NEAR_Z));
        if (!posCS4_n[3]) return outRayCS;

        const auto posCS_n  = Vec3(posCS4_n) / posCS4_n[3];

        outRayCS.mSta       = posCS_n;

        const auto posCS4_f = XFormPos4(xf_CS_PS, Vec3(posNS[0], posNS[1], FAR_Z));
        if (!posCS4_f[3]) return outRayCS;

        auto posCS_f  = Vec3(posCS4_f) / posCS4_f[3];

        outRayCS.mDir = glm::normalize(posCS_f - posCS_n);

        return outRayCS;
    }

} // namespace ge

#endif
