#include "storm/option/Option.hpp"
#include "storm/option/Options.hpp"
#include "storm/Error.hpp"

int32_t StormSetOption(int32_t optname, const void* optval, uint32_t optlen) {
    SErrSetLastError(ERROR_INVALID_PARAMETER);
    if (!optval) {
        return 0;
    }

    uint32_t val;

    switch (optname) {
    case 1:
        if (optlen != 4) {
            return 0;
        }
        g_opt.serrleaksilentwarning = *static_cast<const int32_t*>(optval);
        SErrSetLastError(0);
        return 1;
    case 2:
        val = *static_cast<const int32_t*>(optval);
        if (optlen != 4 || !val || (val ^ (val - 1)) != 2 * val - 1) {
            return 0;
        }
        g_opt.wavechunksize = val;
        SErrSetLastError(0);
        return 1;
    case 3:
        if (optlen != 4) {
            return 0;
        }
        g_opt.smemleaksilentwarning = *static_cast<const int32_t*>(optval);
        SErrSetLastError(0);
        return 1;
    case 4:
        if (optlen != 4) {
            return 0;
        }
        g_opt.opt0C = *static_cast<const int32_t*>(optval);
        SErrSetLastError(0);
        return 1;
    case 5:
        if (optlen != 4) {
            return 0;
        }
        g_opt.opt10 = *static_cast<const int32_t*>(optval);
        SErrSetLastError(0);
        return 1;
    case 6:
        if (optlen != 8) {
            return 0;
        }
        s_alloccount = static_cast<const uint32_t*>(optval)[0];
        s_freecount  = static_cast<const uint32_t*>(optval)[1];
        SErrSetLastError(0);
        return 1;
    case 7:
        if (optlen != 4) {
            return 0;
        }
        g_opt.opt14 = *static_cast<const int32_t*>(optval);
        SErrSetLastError(0);
        return 1;
    case 9:
        if (optlen != 4) {
            return 0;
        }
        g_opt.opt18 = *static_cast<const int32_t*>(optval) != 0;
        SErrSetLastError(0);
        return 1;
    case 10:
        if (optlen != 4) {
            return 0;
        }
        g_opt.opt1C = *static_cast<const int32_t*>(optval) != 0;
        SErrSetLastError(0);
        return 1;
    case 11:
        if (optlen != 4) {
            return 0;
        }
        g_opt.sregunicode = *static_cast<const int32_t*>(optval) != 0;
        SErrSetLastError(0);
        return 1;
    case 12:
        if (optlen != 4) {
            return 0;
        }
        g_opt.opt24 = *static_cast<const int32_t*>(optval) != 0;
        SErrSetLastError(0);
        return 1;
    case 13:
        if (optlen != 4) {
            return 0;
        }
        g_opt.opt28 = *static_cast<const int32_t*>(optval) != 0;
        SErrSetLastError(0);
        return 1;
    default:
        return 0;
    }
}

int32_t StormGetOption(int32_t optname, void* optval, uint32_t* optlen) {
    // TODO
    return 0;
}
