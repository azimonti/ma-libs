/************************/
/*      light.cpp       */
/*    Version 1.0       */
/*     2022/06/19       */
/************************/

#include "light.h"
#include "shaderprogram.h"

void ge::DirLight::SetShaderUniforms(const ShaderProgram& shader, const glm::mat4& mat_CS_WS) const
{
    shader.SetUniform("u_lightDirCS", glm::normalize(glm::mat3(mat_CS_WS) * mDirWS));
    shader.SetUniform("u_lightDiffCol", mDiffuseCol);
    shader.SetUniform("u_lightAmbCol", mAmbientCol);
}
