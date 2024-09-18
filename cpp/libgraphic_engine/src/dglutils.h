#ifndef _DGLUTILS_H_3AAD02AE3147458AAD83342D1D5B331E_
#define _DGLUTILS_H_3AAD02AE3147458AAD83342D1D5B331E_

/************************/
/*    dglutils.h        */
/*    Version 1.0       */
/*     2022/06/16       */
/************************/

static const unsigned int INVALID_OGL_OBJ_ID = (unsigned int)-1;

namespace DGLUT
{

    bool MasterCheckGLErr(const char* pFileName, int line);
    bool CheckGLErr(const char* pFileName, int line, bool doPrint = true);
    void FlushGLErr();

    const char* GetFBStatusStr(unsigned int status);

    bool SetupErrorIntercept();

} // namespace DGLUT

#if defined(DEBUG) || defined(_DEBUG)
#define MASTERCHECKGLERR DGLUT::MasterCheckGLErr(__FILE__, __LINE__)
#define CHECKGLERR       DGLUT::CheckGLErr(__FILE__, __LINE__)
#define FLUSHGLERR       DGLUT::FlushGLErr()
#else
#define MASTERCHECKGLERR
#define CHECKGLERR
#define FLUSHGLERR
#endif

#endif
