#ifndef _GEOMPLANES_H_CD68509FA9B549E39DD4FA14F2661F12_
#define _GEOMPLANES_H_CD68509FA9B549E39DD4FA14F2661F12_

/************************/
/*    geomplanes.h      */
/*    Version 1.0       */
/*     2022/06/18       */
/************************/

#include <stdexcept>
#include <string>
#include <vector>

namespace gegt
{
    enum PLANES : size_t { XY = 0, XZ, YZ, PLANELAST };

    inline std::string GetPlaneName(size_t p)
    {
        switch (p)
        {
        case XY: return std::string("XY");
        case XZ: return std::string("XZ");
        case YZ: return std::string("YZ");
        default: throw std::invalid_argument("Unknown plane");
        }
    }

    inline std::vector<std::string> GetAllPlanesName()
    {
        std::vector<std::string> v;
        for (size_t i = XY; i < PLANELAST; ++i) v.push_back(GetPlaneName(i));
        return v;
    }

} // namespace gegt
#endif
