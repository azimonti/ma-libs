#ifndef _GE_RAY_H_6CA03F3FBD6A4C078A67BF91F8B90226_
#define _GE_RAY_H_6CA03F3FBD6A4C078A67BF91F8B90226_

/************************/
/*       ge_ray.h       */
/*    Version 1.0       */
/*     2022/06/29       */
/************************/

#include <cassert>
#include "ge_mathbase.h"
#include "transform.h"

namespace ge
{

    class Ray
    {
      public:
        Vec3 mSta;
        Vec3 mDir;

        Ray(bool doInitialize = false)
        {
            if (doInitialize)
            {
                mSta = {0, 0, 0};
                mDir = {0, 0, 0};
            }
        }

        Ray(const Vec3& sta, const Vec3& dir) : mSta(sta), mDir(dir) {}

        bool IsValid() const
        {
#ifdef DEBUG
            auto isBadF3 = [](const Vec3& x) { return x[0] != x[0] || x[1] != x[1] || x[2] != x[2]; };
            assert(!isBadF3(mSta) && !isBadF3(mDir));
#endif
            return 0 != mDir[0] || 0 != mDir[1] || 0 != mDir[2];
        }

        template <bool DO_NORMALIZE> Ray CalcTransformed(const Transform& xf) const
        {
            if (DO_NORMALIZE) return {xf.XFormPos(mSta), xf.XFormNorN(mDir)};
            else return {xf.XFormPos(mSta), xf.XFormNor(mDir)};
        }
    };

} // namespace ge

#endif
