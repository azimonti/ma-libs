#ifndef _CAMERA_H_C396BB49D04B4E14AA868E5FD0A5C771_
#define _CAMERA_H_C396BB49D04B4E14AA868E5FD0A5C771_

/************************/
/*      camera.h        */
/*    Version 1.0       */
/*     2022/06/16       */
/************************/

#include <string>
#include "transform.h"

namespace ge
{

    class Camera
    {
      public:
        const Transform& GetTransform() const { return mXForm_WS_OS; }

        Transform& GetTransform() { return mXForm_WS_OS; }

        glm::mat4 CalcProjMatrix() const
        {
            if (mFOV > 0.f) return glm::perspective(mFOV / 180.f * glm::pi<float>(), mAspect_woh, mNear, mFar);
            else
                return glm::ortho(-mAspect_woh * mOrthoScale, mAspect_woh * mOrthoScale, -mOrthoScale, mOrthoScale,
                                  mNear, mFar);
        }

        float mNear       = 0.01f;
        float mFar        = 10000.f;
        float mFOV        = 50.f;
        float mAspect_woh = 1.f;
        float mOrthoScale = 1.f;

      private:
        std::string mName;
        Transform mXForm_WS_OS;
    };
} // namespace ge
#endif
