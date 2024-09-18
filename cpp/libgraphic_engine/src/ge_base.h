#ifndef _GE_BASE_H_6097F4E2A14F44D1AEB1EA4C31C770E7_
#define _GE_BASE_H_6097F4E2A14F44D1AEB1EA4C31C770E7_

/************************/
/*      ge_base.h       */
/*    Version 1.0       */
/*     2022/06/29       */
/************************/

namespace ge
{

    // WARNING: should match with streams in ShaderProgram
    enum VtxType { VTX_POS, VTX_TC0, VTX_COL, VTX_NOR, VTX_N };

    static constexpr unsigned int VTX_FLG_NONE = 0;
    static constexpr unsigned int VTX_FLG_POS  = 1 << VTX_POS;
    static constexpr unsigned int VTX_FLG_TC0  = 1 << VTX_TC0;
    static constexpr unsigned int VTX_FLG_COL  = 1 << VTX_COL;
    static constexpr unsigned int VTX_FLG_NOR  = 1 << VTX_NOR;
    static constexpr unsigned int VTX_FLG_ALL  = (1 << VTX_N) - 1;

} // namespace ge

#endif
