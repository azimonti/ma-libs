#ifndef _GE_BBOX_H_53A718F4FC204753BB2F0BC935A55555_
#define _GE_BBOX_H_53A718F4FC204753BB2F0BC935A55555_

/************************/
/*      ge_bbox.h       */
/*    Version 1.0       */
/*     2022/06/29       */
/************************/

#include <array>
#include <cassert>
#include <limits>
#include "ge_mathbase.h"

namespace ge
{

    class BBox
    {
      public:
        Vec3 mMin;
        Vec3 mMax;

        BBox(bool doInitialize = false)
        {
            if (doInitialize) ResetBBox();
        }

        BBox(const Vec3& minVal, const Vec3& maxVal) : mMin(minVal), mMax(maxVal) {}

        BBox(const Vec3 minMax[2]) : mMin(minMax[0]), mMax(minMax[1]) {}

        void ResetBBox()
        {
            mMin = Vec3(FLT_MAX, FLT_MAX, FLT_MAX);
            mMax = Vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
        }

        static inline BBox MakeFromSegment(const Vec3& segSta, const Vec3& segEnd)
        {
            auto bbox = BBox();
            bbox.mMin = bbox.mMax = segSta;
            bbox.UpdateBBox(segEnd);
            return bbox;
        }

        bool IsValidBBox() const { return mMin[0] <= mMax[0] && mMin[1] <= mMax[1] && mMin[2] <= mMax[2]; }

        inline const Vec3& UpdateBBox(const Vec3& val)
        {
            mMin = glm::min(mMin, val);
            mMax = glm::max(mMax, val);
            return val;
        }

        inline void UpdateBBox(const BBox& bbox)
        {
            mMin = glm::min(mMin, bbox.mMin);
            mMax = glm::max(mMax, bbox.mMax);
        }

        bool Contains(const Vec3& vtx) const
        {
            return vtx[0] >= mMin[0] && vtx[1] >= mMin[1] && vtx[2] >= mMin[2] && vtx[0] <= mMax[0] &&
                   vtx[1] <= mMax[1] && vtx[2] <= mMax[2];
        }

        bool Intersects(const BBox& other) const
        {
            return !(other.mMax[0] < mMin[0] || other.mMax[1] < mMin[1] || other.mMax[2] < mMin[2] ||
                     other.mMin[0] > mMax[0] || other.mMin[1] > mMax[1] || other.mMin[2] > mMax[2]);
        }

        Vec3 CalcSize() const { return mMax - mMin; }

        Vec3 CalcCenter() const { return (mMax + mMin) * 0.5f; }

        float CalcRadius() const { return glm::length(mMax - CalcCenter()); }

        float CalcRadiusSqr() const { return LengthSqr(mMax - CalcCenter()); }

        std::array<Vec3, 8> MakeCubeVerts() const;

        const Vec3& operator[](size_t idx) const
        {
            assert(idx <= 1);
            return *(&mMin + idx);
        }

        Vec3& operator[](size_t idx)
        {
            assert(idx <= 1);
            return *(&mMin + idx);
        }
    };

    inline std::array<Vec3, 8> BBox::MakeCubeVerts() const
    {
        /*
                y          2 ----- 6           __________
                |__ x    / |     / |          /|        /|
               /        3 -+--- 7  |         /_|______ / |
              z         |  0 ---+- 4        |  |______|__|
                        |/      |/          | /       | /
                        1 ----- 5           |/________|/
        */
        return std::array<Vec3, 8>{
            mMin,
            {mMin[0], mMin[1], mMax[2]},
            {mMin[0], mMax[1], mMin[2]},
            {mMin[0], mMax[1], mMax[2]},
            {mMax[0], mMin[1], mMin[2]},
            {mMax[0], mMin[1], mMax[2]},
            {mMax[0], mMax[1], mMin[2]},
            mMax
        };
    }

} // namespace ge

#endif
