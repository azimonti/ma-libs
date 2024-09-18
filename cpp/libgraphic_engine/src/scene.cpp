/************************/
/*      scene.cpp       */
/*    Version 1.0       */
/*     2022/06/12       */
/************************/

#include "ge_base.h"
#include "ge_geomutils.h"
#include "ge_ray.h"
#include "scene.h"

namespace ge
{

    void RendState::ResetState()
    {
        mIsPolyOffset = false;
        glDisable(GL_POLYGON_OFFSET_FILL);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
    }

    void RendState::SetPolyOffset(bool onOff)
    {
        if (mIsPolyOffset == onOff) return;

        mIsPolyOffset = onOff;
        if (mIsPolyOffset)
        {
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.f, 3.f);
        }
        else { glDisable(GL_POLYGON_OFFSET_FILL); }
    }

    Scene::Scene()
    {
        for (size_t tc0Idx = 0; tc0Idx < 2; ++tc0Idx)
            for (size_t colIdx = 0; colIdx < 2; ++colIdx)
                for (size_t norIdx = 0; norIdx < 2; ++norIdx) ctor_MakeShader(tc0Idx, colIdx, norIdx);
    }

    void Scene::ctor_MakeShader(size_t tc0Idx, size_t colIdx, size_t norIdx)
    {
        std::string header = R"RAW(
            #version 400

            // light source uniforms
            uniform vec3 u_lightDirCS;
            uniform vec3 u_lightDiffCol;
            uniform vec3 u_lightAmbCol;
            // material uniforms
            uniform vec3  u_mate_DiffuseCol;
            uniform vec3  u_mate_SpecularCol;
            uniform float u_mate_ShininessPow;
        )RAW";

        header += "#define VTX_POS " + std::to_string(VTX_POS) + "\n";
        header += "#define VTX_TC0 " + std::to_string(VTX_TC0) + "\n";
        header += "#define VTX_COL " + std::to_string(VTX_COL) + "\n";
        header += "#define VTX_NOR " + std::to_string(VTX_NOR) + "\n";

        if (tc0Idx) header += "#define USE_TC0\n";
        if (colIdx) header += "#define USE_COL\n";
        if (norIdx) header += "#define USE_NOR\n";

        const std::string vertexShader   = R"RAW(
        uniform mat4 u_mtxPS_OS;
        uniform mat4 u_mtxWS_OS;
        uniform mat4 u_mtxCS_OS;
        layout(location = VTX_POS) in vec3 a_pos;
    #ifdef USE_TC0
        layout(location = VTX_TC0) in vec4 a_tc0;
    #endif
    #ifdef USE_COL
        layout(location = VTX_COL) in vec4 a_col;
    #endif
        out vec4 v_col;
    #ifdef USE_NOR
        layout(location = VTX_NOR) in vec3 a_nor;
        out vec3 v_norCS;
        out vec3 v_posCS;
    #endif
        void main()
        {
            gl_Position = u_mtxPS_OS * vec4(a_pos, 1.0);
    #ifdef USE_COL
            v_col = a_col;
    #else
            v_col = vec4(1.0);
    #endif
    #ifdef USE_NOR
            v_norCS = mat3( u_mtxCS_OS ) * a_nor;
            v_posCS = (u_mtxCS_OS * vec4( a_pos, 1.0 )).xyz;
    #endif
        }
    )RAW";

        const std::string fragmentShader = R"RAW(
        layout(location = 0) out vec4 color;
        in vec4 v_col;
    #ifdef USE_NOR
        in vec3 v_norCS;
        in vec3 v_posCS;
    #endif

        float calcSpecCoe( in vec3 nor, in vec3 H, in float shininess )
        {
            const float PI = 3.1415926535897932384626433832795;
            float BLINN_PHONG_ENERGY_CONSERV = (8.0 + shininess) / (8.0 * PI);

            return BLINN_PHONG_ENERGY_CONSERV * pow( max( dot( nor, H ), 0.0 ), shininess );
        }

        float calcFresnel( in vec3 V, in vec3 H )
        {
            return pow( 1.0 - dot( H, V ), 5.0 );
        }

        vec3 calcLighting(
                in vec3 rawNor,
                in vec3 pos,
                in vec3 mateDiffCol,
                in vec3 mateSpecCol,
                in float mateShininess )
        {
            vec3 nor = normalize( rawNor );
            vec3 L = u_lightDirCS;

            // col = diffuse
            float NdotL = max( dot( nor, L ), 0.0 );
            vec3 dirLightCol = u_lightDiffCol * NdotL;
            vec3 col = (u_lightAmbCol + dirLightCol) * mateDiffCol;

            // col += specular
            vec3 V = normalize( -pos );
            vec3 H = normalize( V + L );

            float specCoe = calcSpecCoe( nor, H, mateShininess );
            float fresVal = calcFresnel( V, H );

            specCoe *= 1.0 - fresVal;

            col += mateSpecCol * specCoe;

            return col;
        }

        void main(){
    #ifdef USE_NOR
           float NdotL = dot( normalize( v_norCS ), u_lightDirCS );

           vec3 col3 = v_col.xyz * calcLighting(
                                        v_norCS,
                                        v_posCS,
                                        u_mate_DiffuseCol,
                                        u_mate_SpecularCol,
                                        u_mate_ShininessPow );

           color = vec4( col3, v_col.w );
    #else
           color = v_col;
    #endif
        }
    )RAW";

        mShaders[tc0Idx][colIdx][norIdx].LoadProgram((header + vertexShader).c_str(), GL_VERTEX_SHADER,
                                                     (header + fragmentShader).c_str(), GL_FRAGMENT_SHADER, NULL);
    }

    void Scene::BeginRender(const ge::RendSetup& rsetup)
    {
        mPick_Active  = false;
        mCurMat_CS_WS = rsetup.mpCamera->GetTransform().GetMatrix();
        mCurMat_PS_WS = rsetup.mpCamera->CalcProjMatrix() * mCurMat_CS_WS;

        mRendState.ResetState();
    }

    void Scene::BeginPicking(const RendSetup& rsetup, float x, float y)
    {
        mPick_Active  = true;
        mCurMat_CS_WS = rsetup.mpCamera->GetTransform().GetMatrix();
        mCurMat_PS_WS = rsetup.mpCamera->CalcProjMatrix() * mCurMat_CS_WS;

        // pass camera-space -> projective-space
        mPick.BeginPicking(rsetup, x, y, mCurMat_CS_WS * glm::inverse(mCurMat_PS_WS));
    }

    ScenePickData Scene::EndPicking()
    {
        return mPick.EndPicking(mCurMat_CS_WS);
    }

    void Scene::addMeshRender(const Mesh& mesh)
    {
        const auto& mat_WS_OS = mesh.GetTransform().GetMatrix();
        const auto mat_PS_OS  = mCurMat_PS_WS * mat_WS_OS;

        const auto hasCol     = 1u & mesh.HasCol();
        const auto hasNor     = 1u & mesh.HasNor();
        const auto& shader    = mShaders[0][hasCol][hasNor];

        shader.UseProgram();
        shader.SetUniform("u_mtxPS_OS", mat_PS_OS);
        shader.SetUniform("u_mtxWS_OS", mat_WS_OS);
        shader.SetUniform("u_mtxCS_OS", mCurMat_CS_WS * mat_WS_OS);

        // set uniforms for the mesh material
        mesh.GetMaterial().SetShaderUniforms(shader);
        // see if we have to set the polygon offset
        mRendState.SetPolyOffset(mesh.GetMaterial().mUsePolyOffset);

        // set uniforms for the light source
        mDirLight.SetShaderUniforms(shader, mCurMat_CS_WS);

        mesh.DrawMesh();
    }

    void Scene::AddMesh(const Mesh& mesh)
    {
        if (mPick_Active) mPick.AddMeshPick(mesh, mCurMat_CS_WS);
        else addMeshRender(mesh);
    }

} // namespace ge
