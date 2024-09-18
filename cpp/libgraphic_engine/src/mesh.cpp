/************************/
/*     mesh.cpp         */
/*    Version 1.0       */
/*     2022/06/12       */
/************************/

#include <glm/gtx/normal.hpp>
#include <stdarg.h>
#include "dglutils.h"
#include "ge_base.h"
#include "mesh.h"
#include "shaderprogram.h"

namespace ge
{

    void Material::SetShaderUniforms(const ShaderProgram& shader) const
    {
        shader.SetUniform("u_mate_DiffuseCol", mDiffuseCol);
        shader.SetUniform("u_mate_SpecularCol", mSpecularCol);
        shader.SetUniform("u_mate_ShininessPow", mShininessPow);
    }

    Mesh::Mesh(bool isTriangles) : mIsTriangles(isTriangles) {}

    Mesh::~Mesh()
    {
        if (mVAO) glDeleteVertexArrays(1, &mVAO);
        if (mPosBuff) glDeleteBuffers(1, &mPosBuff);
        if (mColBuff) glDeleteBuffers(1, &mColBuff);
        if (mNorBuff) glDeleteBuffers(1, &mNorBuff);
        if (mIdxBuff) glDeleteBuffers(1, &mIdxBuff);
    }

    template <typename T>
    static void makeArrBuffer(const std::vector<T>& vec, GLenum type, GLuint& io_buff, GLuint attribIdx, int dimsN,
                              bool normalize)
    {
        glEnableVertexAttribArray(attribIdx);
        glGenBuffers(1, &io_buff);
        glBindBuffer(type, io_buff);
        glVertexAttribPointer(attribIdx, dimsN, GL_FLOAT, normalize, sizeof(vec[0]), 0);
    }

    void Mesh::InitializeGeometryAttributes(bool hasIdx, unsigned int vtxFlags)
    {
        assert(!mVAO && !mPosBuff && !mColBuff && !mNorBuff && !mIdxBuff);

        glGenVertexArrays(1, &mVAO);
        glBindVertexArray(mVAO);

        if (vtxFlags & VTX_FLG_POS) makeArrBuffer(mPos, GL_ARRAY_BUFFER, mPosBuff, VTX_POS, 3, false);
        if (vtxFlags & VTX_FLG_COL) makeArrBuffer(mCol, GL_ARRAY_BUFFER, mColBuff, VTX_COL, 4, false);
        if (vtxFlags & VTX_FLG_NOR) makeArrBuffer(mNor, GL_ARRAY_BUFFER, mNorBuff, VTX_NOR, 3, true);

        glBindVertexArray(0);

        // element array (index buffer)
        if (hasIdx) glGenBuffers(1, &mIdxBuff);
    }

    inline auto updateBuff = [](auto& vec, const GLenum type, const GLenum usage, const auto& buff, auto& curSiz,
                                bool bind) {
        const auto newSize = sizeof(vec[0]) * vec.size();
        if (!newSize) return;

        if (bind)
        {
            glBindBuffer(type, buff);
            CHECKGLERR;
        }

        if (newSize > curSiz) // expand as necessary
        {
            curSiz = newSize;
            glBufferData(type, (GLsizeiptr)newSize, 0, usage);
            CHECKGLERR;
        }
        glBufferSubData(type, 0, (GLsizeiptr)newSize, vec.data());
        if (bind)
        {
            glBindBuffer(type, 0);
            CHECKGLERR;
        }
    };

    void Mesh::UpdateBuffers(bool makeDynamic)
    {
        const auto usage = makeDynamic ? (GLenum)GL_DYNAMIC_DRAW : (GLenum)GL_STATIC_DRAW;
        updateBuff(mPos, GL_ARRAY_BUFFER, usage, mPosBuff, mCurVBOSizes[VTX_POS], true);
        updateBuff(mCol, GL_ARRAY_BUFFER, usage, mColBuff, mCurVBOSizes[VTX_COL], true);
        updateBuff(mNor, GL_ARRAY_BUFFER, usage, mNorBuff, mCurVBOSizes[VTX_TC0], true);
        updateBuff(mIdx, GL_ELEMENT_ARRAY_BUFFER, usage, mIdxBuff, mCurVAESize, true);
        mUsedVBOSizeN = mPos.size();
        mUsedVAESizeN = mIdx.size();
        if (makeDynamic)
        {
            mPos.clear();
            mCol.clear();
            mNor.clear();
            mIdx.clear();
        }
    }

    void Mesh::UpdateBoundingBox()
    {
        mBBoxOS.ResetBBox();
        if (!mIdx.empty())
        {
            for (const auto& idx : mIdx) mBBoxOS.UpdateBBox(mPos[idx]);
        }
        else
        {
            for (const auto& p : mPos) mBBoxOS.UpdateBBox(p);
        }
    }

    void Mesh::OnGeometryUpdate()
    {
        // initialize once here if it's not already defined
        if (!mPosBuff)
        {
            InitializeGeometryAttributes(!mIdx.empty(), (mPos.empty() ? VTX_FLG_NONE : VTX_FLG_POS) |
                                                            (mCol.empty() ? VTX_FLG_NONE : VTX_FLG_COL) |
                                                            (mNor.empty() ? VTX_FLG_NONE : VTX_FLG_NOR));
        }

        // update the buffers (non-dynamic for this kind of approach)
        UpdateBuffers(false);

        // we also do this here automatically
        UpdateBoundingBox();
    }

    void Mesh::DrawMesh() const
    {
        const auto primType = mIsTriangles ? GL_TRIANGLES : GL_LINES;

        glBindVertexArray(mVAO);

        if (mUsedVAESizeN)
        {
            const auto n = (GLsizei)mUsedVAESizeN;
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIdxBuff);
            glDrawElements((GLenum)primType, n, GL_UNSIGNED_INT, nullptr);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        else
        {
            const auto n = (GLsizei)mUsedVBOSizeN;
            glDrawArrays((GLenum)primType, 0, n);
        }

        glBindVertexArray(0);
    }

} // namespace ge
