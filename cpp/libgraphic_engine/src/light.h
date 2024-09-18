#ifndef _LIGHT_H_ABD965FF5F554524BFC7E03370BDD287_
#define _LIGHT_H_ABD965FF5F554524BFC7E03370BDD287_

/************************/
/*       light.h        */
/*    Version 1.0       */
/*     2022/06/19       */
/************************/

#include "ge_mathbase.h"

namespace ge
{
    class ShaderProgram;

    class DirLight
    {
      public:
        glm::vec3 mDirWS{0, 0, 1};
        glm::vec3 mDiffuseCol{1, 1, 1};
        glm::vec3 mAmbientCol{0.3f, 0.3f, 0.3f};

        void SetDirWS(const glm::vec3& dirWS) { mDirWS = dirWS; }

        const glm::vec3& GetDirWS() const { return mDirWS; }

        void SetDiffuseCol(const glm::vec3& diffuseColor) { mDiffuseCol = diffuseColor; }

        const glm::vec3& GetDiffuseCol() const { return mDiffuseCol; }

        void SetShaderUniforms(const ShaderProgram& shader, const glm::mat4& mat_CS_WS) const;
    };
} // namespace ge

#endif
