#ifndef _SCENE_H_153230F198324DF2BBB5CF0A4C366A56_
#define _SCENE_H_153230F198324DF2BBB5CF0A4C366A56_

/************************/
/*      scene.h         */
/*    Version 1.0       */
/*     2022/06/12       */
/************************/

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "camera.h"
#include "ge_scenepicking.h"
#include "light.h"
#include "mesh.h"
#include "shaderprogram.h"

namespace ge
{

    class RendSetup
    {
      public:
        Camera* mpCamera{};
        Vec2 mRTargetSiz{};
    };

    class RendState
    {
      public:
        bool mIsPolyOffset{};

        void ResetState();
        void SetPolyOffset(bool onOff);
    };

    class Scene
    {
      public:
        Scene();

        void BeginRender(const RendSetup& rsetup);
        void AddMesh(const Mesh& mesh);

        void BeginPicking(const RendSetup& rsetup, float x, float y);
        ScenePickData EndPicking();

        const DirLight& GetDirLight() const { return mDirLight; }

        DirLight& GetDirLight() { return mDirLight; }

      private:
        void ctor_MakeShader(size_t tc0Idx, size_t colIdx, size_t norIdx);
        void addMeshRender(const Mesh& mesh);

      private:
        ShaderProgram mShaders[2][2][2];
        TransformP mCurMat_PS_WS{};
        Mat4 mCurMat_CS_WS{};
        DirLight mDirLight;

        RendState mRendState;

        bool mPick_Active{};
        ScenePick mPick;

        friend class ScenePick;
    };

} // namespace ge

#endif
