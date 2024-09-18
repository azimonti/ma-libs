#ifndef _SHADERPROGRAM_H_78D467B1D88F4970A28876EB2D09B8A6_
#define _SHADERPROGRAM_H_78D467B1D88F4970A28876EB2D09B8A6_

/************************/
/*  shaderprogram.h     */
/*    Version 1.0       */
/*     2022/06/19       */
/************************/

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace ge
{
    using uint = unsigned int;

    class ShaderProgram
    {
      public:
        ~ShaderProgram();

        void LoadProgram(const char* shader, ...);
        void UseProgram() const;

        uint GetProgramID() const { return mProgramID; }

        void SetUniform(const char* pName, float v) const;
        void SetUniform(const char* pName, const glm::mat4& m) const;
        void SetUniform(const char* pName, const glm::vec3& v) const;
        void SetUniform(const char* pName, const glm::vec4& v) const;

      private:
        uint mProgramID{};
    };
} // namespace ge

#endif
