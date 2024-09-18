/************************/
/*    dglutils.cpp      */
/*    Version 1.0       */
/*     2022/06/16       */
/************************/

#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdarg.h>
#include "log/log.h"
#include "dglutils.h"

// #define USE_LAZY_FAST_ERR_CHECK

#if defined(GL_ARB_debug_output) && !defined(__linux__)
static constexpr bool INTERCEPT_LOW = false;
static constexpr bool INTERCEPT_MED = false;
static constexpr bool INTERCEPT_NOT = false;
#endif

namespace DGLUT
{

    inline std::string SSPrintFS(const char* format, ...)
    {
        va_list args;
        va_start(args, format);

        const auto size = (size_t)vsnprintf(nullptr, 0, format, args) + 1;
        std::unique_ptr<char[]> buf(new char[size]);
        vsnprintf(buf.get(), size, format, args);

        va_end(args);

        return std::string(buf.get(), buf.get() + size - 1);
    }

    static auto localLogErr = [](const char* ftm, ...) {
        char buffer[2048]{};
        va_list args;
        va_start(args, ftm);
        vsnprintf(buffer, sizeof(buffer), ftm, args);
        va_end(args);
        // LOGGER(logging::ERROR) << buffer;
        printf("ERROR: %s\n", buffer);
    };

#ifdef USE_LAZY_FAST_ERR_CHECK
    static bool _sMasterCheck;
#endif

    static const char* getErrStr(GLenum err)
    {
        switch (err)
        {
        case GL_NO_ERROR: return "GL_NO_ERROR";
        case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
        case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
        default: {
            static std::string unkStr;
            unkStr = SSPrintFS("#x%04x", err);
            return unkStr.c_str();
        }
        }
    }

    const char* GetFBStatusStr(unsigned int status)
    {
        switch (status)
        {
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
        case GL_FRAMEBUFFER_UNSUPPORTED: return "GL_FRAMEBUFFER_UNSUPPORTED";

#ifdef GL_FRAMEBUFFER_UNDEFINED
        case GL_FRAMEBUFFER_UNDEFINED: return "GL_FRAMEBUFFER_UNDEFINED";
#endif
#ifdef GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
#endif
#ifdef GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
#endif
#ifdef GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: return "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
#endif
#ifdef GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: return "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
#endif

        default: {
            static std::string unkStr;
            unkStr = SSPrintFS("#x%04x", status);
            return unkStr.c_str();
        }
        }
    }

    bool MasterCheckGLErr(const char* pFileName, int line)
    {
#if defined(USE_LAZY_FAST_ERR_CHECK)
        bool didErr = false;
        DVec<GLenum> errList;
        GLenum err = glGetError();
        while (err != GL_NO_ERROR)
        {
            didErr = true;
            errList.push_back(err);
            err = glGetError();
        }

        if (errList.size())
        {
            if NOT (_sMasterCheck)
            {
                _sMasterCheck = true;

                localLogErr("Master Error Check found %zu errors. Activating detailed check.", errList.size());
            }

            std::string errStrList;
            for (auto err : errList)
            {
                errStrList += getErrStr(err);
                errStrList += ' ';
            }
            localLogErr("[%s:%i] %s", pFileName, line, errStrList.c_str());
        }
        else { _sMasterCheck = false; }

        return didErr;
#else
        return CheckGLErr(pFileName, line);
#endif
    }

    bool CheckGLErr(const char* pFileName, int line, bool doPrint)
    {
#if defined(USE_LAZY_FAST_ERR_CHECK)
        if NOT (_sMasterCheck) return false;
#endif
        (void)doPrint;

        bool didErr = false;
        GLenum err  = glGetError();
        while (err != GL_NO_ERROR)
        {
            didErr              = true;
            const char* pErrStr = getErrStr(err);

            if (pErrStr) { localLogErr("GL error: %s at %s : %i", pErrStr, pFileName, line); }
            else { localLogErr("Unknown error: %d 0x%x at %s : %i", err, err, pFileName, line); }

            err = glGetError();
        }

        return didErr;
    }

    void FlushGLErr()
    {
        while (glGetError() != GL_NO_ERROR) {}
    }

#if defined(GL_ARB_debug_output) && !defined(__linux__)
    static void GLAPIENTRY errorCallbackARB(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                            const GLchar* pMessage, const void* pUserParam)
    {
        (void)length;
        (void)pUserParam;
        const char* pSource = "";
        switch (source)
        {
        case GL_DEBUG_SOURCE_API: pSource = "API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: pSource = "Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: pSource = "Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY: pSource = "3rd Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION: pSource = "App"; break;
        case GL_DEBUG_SOURCE_OTHER: pSource = "Other"; break;
        default: pSource = "Unknown"; break;
        }

        const char* pType = "";
        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR: pType = "Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: pType = "Deprecated"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: pType = "Undefined"; break;
        case GL_DEBUG_TYPE_PORTABILITY: pType = "Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE: pType = "Performance"; break;
        case GL_DEBUG_TYPE_OTHER: pType = "Other"; break;
        default: pType = "Unknown"; break;
        }

        const char* pSeverity = "";
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH: pSeverity = "H"; break;
        case GL_DEBUG_SEVERITY_MEDIUM: pSeverity = "M"; break;
        case GL_DEBUG_SEVERITY_LOW: pSeverity = "L"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: pSeverity = "Not"; break;
        default: pSeverity = "Unk"; break;
        }

        (void)pSource;
        (void)id;
        localLogErr("* GLERR: %s(%s) -- %s", pType, pSeverity, pMessage);
    }
#endif

    bool SetupErrorIntercept()
    {
#if defined(GL_ARB_debug_output) && !defined(__linux__)
        if (glDebugMessageCallback)
        {
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(errorCallbackARB, NULL);

            auto enableServerity = [](auto sev, bool onOff) {
                glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, (GLenum)sev, 0, nullptr, (GLboolean)onOff);
            };

            enableServerity(GL_DONT_CARE, true);
            enableServerity(GL_DEBUG_SEVERITY_LOW, INTERCEPT_LOW);
            enableServerity(GL_DEBUG_SEVERITY_MEDIUM, INTERCEPT_MED);
            enableServerity(GL_DEBUG_SEVERITY_NOTIFICATION, INTERCEPT_NOT);

            return true;
        }
#endif

        return false;
    }

} // namespace DGLUT
