#ifndef _SCENE_H_3D786837DAE8467BB786E5F67D8C428F_
#define _SCENE_H_3D786837DAE8467BB786E5F67D8C428F_

/************************/
/*       mesh.h         */
/*    Version 1.0       */
/*     2022/06/12       */
/************************/

#include <array>
#include <vector>
#include <GL/glew.h>
#include "ge_base.h"
#include "ge_bbox.h"
#include "ge_mathbase.h"
#include "transform.h"

namespace ge
{
    class ShaderProgram;

    class Material
    {
      public:
        glm::vec3 mDiffuseCol{1, 1, 1};
        glm::vec3 mSpecularCol{0.3f, 0.3f, 0.3f};
        float mShininessPow{80};
        bool mUsePolyOffset{};

        void SetShaderUniforms(const ShaderProgram& shader) const;
    };

    class Mesh
    {
      public:
        Mesh(bool isTriangles = true);
        ~Mesh();
        void InitializeGeometryAttributes(bool hasIdx, unsigned int vtxFlags);
        void OnGeometryUpdate();
        void UpdateBuffers(bool makeDynamic = false);
        void UpdateBoundingBox();
        void DrawMesh() const;

        const Transform& GetTransform() const { return mXForm_WS_OS; }

        Transform& GetTransform() { return mXForm_WS_OS; }

        const Material& GetMaterial() const { return mMaterial; }

        Material& GetMaterial() { return mMaterial; }

        bool IsTriangles() const { return mIsTriangles; }

        void SetObjMatrix(glm::mat4 m) { mXForm_WS_OS.SetMatrix(m); }

        void RotateObjMatrix(float f, glm::vec3 v) { mXForm_WS_OS.RotateByAxis(f, v); }

        void TranslateObjMatrix(const glm::vec3& v) { mXForm_WS_OS.Translate(v); }

        std::vector<uint32_t>& GetIdxVec() { return mIdx; }

        std::vector<glm::vec3>& GetPosVec() { return mPos; }

        std::vector<glm::vec4>& GetColVec() { return mCol; }

        std::vector<glm::vec3>& GetNorVec() { return mNor; }

        const std::vector<uint32_t>& GetIdxVec() const { return mIdx; }

        const std::vector<glm::vec3>& GetPosVec() const { return mPos; }

        const std::vector<glm::vec4>& GetColVec() const { return mCol; }

        const std::vector<glm::vec3>& GetNorVec() const { return mNor; }

        bool HasIdx() const { return !mIdx.empty() || mIdxBuff; }

        bool HasPos() const { return !mPos.empty() || mPosBuff; }

        bool HasCol() const { return !mCol.empty() || mColBuff; }

        bool HasNor() const { return !mNor.empty() || mNorBuff; }

        void SetBBox(const BBox& bbox) { mBBoxOS = bbox; }

        const BBox& GetBBox() const { return mBBoxOS; }

      private:
        const bool mIsTriangles;
        Material mMaterial;
        Transform mXForm_WS_OS;
        GLuint mVAO{};
        // GLuint                  mVBOs[VTX_N]        {};
        size_t mCurVBOSizes[VTX_N]{};
        size_t mUsedVBOSizeN{};
        GLuint mPosBuff{};
        GLuint mColBuff{};
        GLuint mNorBuff{};
        GLuint mIdxBuff{};
        size_t mCurVAESize{};
        size_t mUsedVAESizeN{};
        std::vector<uint32_t> mIdx;
        std::vector<glm::vec3> mPos;
        std::vector<glm::vec4> mCol;
        std::vector<glm::vec3> mNor;
        BBox mBBoxOS{true};
    };
} // namespace ge

#endif
