#ifndef _TRANSFORM_H_1D7892EC6F8348BA8EC7FFEA60A28422_
#define _TRANSFORM_H_1D7892EC6F8348BA8EC7FFEA60A28422_

/************************/
/*    transform.h       */
/*    Version 1.0       */
/*     2022/06/16       */
/************************/

#include "ge_mathbase.h"

namespace ge
{

    inline Vec3 XFormPos(const Mat4& m, const Vec3& v)
    {
        return m * Vec4(v[0], v[1], v[2], 1.f);
    }

    inline Vec3 XFormNor(const Mat4& m, const Vec3& v)
    {
        return m * Vec4(v[0], v[1], v[2], 0.f);
    }

    inline Vec3 XFormNorN(const Mat4& m, const Vec3& v)
    {
        return glm::normalize(XFormNor(m, v));
    }

    inline Vec4 XFormPos4(const Mat4& m, const Vec3& v)
    {
        return m * Vec4(v[0], v[1], v[2], 1.f);
    }

    // transform for affine spaces
    class Transform
    {
      public:
        Transform() {}

        Transform(const Mat4& m) : mMtx(m) {}

        Transform(const Mat3& m, const Vec3& v)
        {
            mMtx    = m;
            mMtx[3] = Vec4(v, 1.0f);
        }

        const Mat4& GetMatrix() const { return mMtx; }

        Mat4& GetMatrix() { return mMtx; }

        void SetMatrix(const Mat4& mtx) { mMtx = mtx; }

        Mat3 GetRotSca() const { return Mat3(mMtx); }

        Vec3 GetTranslation() const { return Vec3(mMtx[3]); }

        Transform GetAffineInverse() const
        {
            const auto invRot = glm::inverse(GetRotSca());
            const auto invTra = invRot * -GetTranslation();
            return {invRot, invTra};
        }

        Transform& RotateByAxis(float f, const Vec3& v)
        {
            mMtx = glm::rotate(mMtx, f, v);
            return *this;
        }

        Transform& RotateByQuat(const glm::quat& q)
        {
            mMtx *= glm::toMat4(q);
            return *this;
        }

        Transform& RotateByMat(const Mat3& m)
        {
            mMtx *= Mat4(m);
            return *this;
        }

        Transform& Translate(const Vec3& v)
        {
            mMtx = glm::translate(mMtx, v);
            return *this;
        }

        friend Transform operator*(const Transform& t1, const Transform& t2) { return t1.mMtx * t2.mMtx; }

        friend Transform operator*(const Transform& t1, const Mat4& mtx2) { return t1 * Transform(mtx2); }

        friend Mat4 operator*(const Mat4& mtx1, const Transform& t2) { return mtx1 * t2.GetMatrix(); }

        Vec3 XFormPos(const Vec3& v) const { return ge::XFormPos(mMtx, v); }

        Vec3 XFormNor(const Vec3& v) const { return ge::XFormNor(mMtx, v); }

        Vec3 XFormNorN(const Vec3& v) const { return ge::XFormNorN(mMtx, v); }

        Vec4 XFormPos4(const Vec3& v) const { return ge::XFormPos4(mMtx, v); }

      private:
        Mat4 mMtx{1.f};
    };

    inline Vec3 XFormPos(const Transform& m, const Vec3& v)
    {
        return m.XFormPos(Vec4(v[0], v[1], v[2], 1.f));
    }

    inline Vec3 XFormNor(const Transform& m, const Vec3& v)
    {
        return m.XFormNor(Vec4(v[0], v[1], v[2], 0.f));
    }

    inline Vec3 XFormNorN(const Transform& m, const Vec3& v)
    {
        return m.XFormNorN(v);
    }

    inline Vec4 XFormPos4(const Transform& m, const Vec3& v)
    {
        return m.XFormPos4(Vec4(v[0], v[1], v[2], 1.f));
    }

    // transform for projective spaces
    using TransformP = Mat4;

} // namespace ge

#endif
