/************************/
/*  shaderprogram.cpp   */
/*    Version 1.0       */
/*     2022/06/19       */
/************************/

#include <iostream>
#include <vector>
#include <assert.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <stdarg.h>
#include "shaderprogram.h"

namespace ge
{

    ShaderProgram::~ShaderProgram()
    {
        if (mProgramID) glDeleteProgram(mProgramID);
    }

    static void checkStatus(uint obj)
    {
        GLint status = GL_FALSE;
        if (glIsShader(obj)) glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
        if (glIsProgram(obj)) glGetProgramiv(obj, GL_LINK_STATUS, &status);
        if (status == GL_TRUE) return;
        GLchar log[1 << 15] = {0};
        if (glIsShader(obj)) glGetShaderInfoLog(obj, sizeof(log), NULL, log);
        if (glIsProgram(obj)) glGetProgramInfoLog(obj, sizeof(log), NULL, log);
        std::cerr << log << std::endl;
        std::exit(EXIT_FAILURE);
    }

    static uint createAndAttach(uint program, GLenum type, const char* src)
    {
        auto shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, NULL);
        glCompileShader(shader);
        checkStatus(shader);
        glAttachShader(program, shader);
        return shader;
    }

    void ShaderProgram::LoadProgram(const char* pShaderSrc, ...)
    {
        assert(!mProgramID);

        auto prog = glCreateProgram();
        va_list args;
        va_start(args, pShaderSrc);

        std::vector<uint> shaderIDs;
        while (pShaderSrc)
        {
            const GLenum type = va_arg(args, GLenum);
            shaderIDs.push_back(createAndAttach(prog, type, pShaderSrc));
            pShaderSrc = va_arg(args, const char*);
        }
        va_end(args);

        glLinkProgram(prog);
        checkStatus(prog);

        // once linking is complete, we can detach & delete
        for (uint s : shaderIDs)
        {
            glDetachShader(prog, s);
            glDetachShader(prog, s);
        }

        mProgramID = prog;
    }

    void ShaderProgram::UseProgram() const
    {
        assert(mProgramID);
        glUseProgram(mProgramID);
    }

    void ShaderProgram::SetUniform(const char* pName, float v) const
    {
        if (auto loc = glGetUniformLocation(mProgramID, pName); loc >= 0) glUniform1f(loc, v);
    }

    void ShaderProgram::SetUniform(const char* pName, const glm::mat4& m) const
    {
        if (auto loc = glGetUniformLocation(mProgramID, pName); loc >= 0)
            glUniformMatrix4fv(loc, 1, false, (const float*)&m[0][0]);
    }

    void ShaderProgram::SetUniform(const char* pName, const glm::vec3& v) const
    {
        if (auto loc = glGetUniformLocation(mProgramID, pName); loc >= 0) glUniform3fv(loc, 1, (const float*)&v[0]);
    }

    void ShaderProgram::SetUniform(const char* pName, const glm::vec4& v) const
    {
        if (auto loc = glGetUniformLocation(mProgramID, pName); loc >= 0) glUniform4fv(loc, 1, (const float*)&v[0]);
    }

} // namespace ge
