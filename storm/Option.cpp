#include "storm/Option.hpp"
#include "storm/Error.hpp"

struct STORMOPTIONS {
    int32_t  smemleaksilentwarning;
    int32_t  serrleaksilentwarning;
    uint32_t wavechunksize;
    int32_t  alignstreamingwavedata;
    int32_t  echotooutputdebugstring;
    int32_t  serrsuppresslogs;
    int32_t  crcenabled;
    int32_t  orderedprintfenabled;
    int32_t  sregunicode;
    int32_t  noreaderrordialog;
    int32_t  assertlogonly;
    /*
    int32_t serrsuppressdialogs;
    int32_t serrignorerecoverable;
    */
};

STORMOPTIONS g_opt;

uint32_t s_alloccount;
uint32_t s_freecount;

void IncrementAllocCount() {
    s_alloccount++;
}

void IncrementFreeCount() {
    s_freecount++;
}

int32_t STORMAPI StormSetOption(int32_t optname, const void* optval, uint32_t optlen) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(optval);
    STORM_VALIDATE_END;

    SErrSetLastError(ERROR_INVALID_PARAMETER);

    uint32_t val;

    switch (optname) {
    case 1:
        if (optlen != 4) return 0;
        g_opt.serrleaksilentwarning = *static_cast<const int32_t*>(optval);
        break;
    case 2:
        if (optlen != 4) return 0;

        val = *static_cast<const uint32_t*>(optval);
        if (!val || (val ^ (val - 1)) != 2 * val - 1) {
            return 0;
        }
        g_opt.wavechunksize = val;
        break;
    case 3:
        if (optlen != 4) return 0;
        g_opt.smemleaksilentwarning = *static_cast<const int32_t*>(optval);
        break;
    case 4:
        if (optlen != 4) return 0;
        g_opt.alignstreamingwavedata = *static_cast<const int32_t*>(optval);
        break;
    case 5:
        if (optlen != 4) return 0;
        g_opt.echotooutputdebugstring = *static_cast<const int32_t*>(optval);
        break;
    case 6:
        if (optlen != 8) return 0;
        s_alloccount = static_cast<const uint32_t*>(optval)[0];
        s_freecount  = static_cast<const uint32_t*>(optval)[1];
        break;
    case 7:
        if (optlen != 4) return 0;
        g_opt.serrsuppresslogs = *static_cast<const int32_t*>(optval);
        break;
    case 9:
        if (optlen != 4) return 0;
        g_opt.crcenabled = *static_cast<const int32_t*>(optval) != 0;
        break;
    case 10:
        if (optlen != 4) return 0;
        g_opt.orderedprintfenabled = *static_cast<const int32_t*>(optval) != 0;
        break;
    case 11:
        if (optlen != 4) return 0;
        g_opt.sregunicode = *static_cast<const int32_t*>(optval) != 0;
        break;
    case 12:
        if (optlen != 4) return 0;
        g_opt.noreaderrordialog = *static_cast<const int32_t*>(optval) != 0;
        break;
    case 13:
        if (optlen != 4) return 0;
        g_opt.assertlogonly = *static_cast<const int32_t*>(optval) != 0;
        break;
#if !defined(WHOA_BAD_STORM_OPTION_ALWAYS_SUCCEEDS)
    default:
        return 0;
#endif
    }
    SErrSetLastError(ERROR_SUCCESS);
    return 1;
}

int32_t STORMAPI StormGetOption(int32_t optname, void* optval, uint32_t* optlen) {
    STORM_VALIDATE_BEGIN;
    STORM_VALIDATE(optval);
    STORM_VALIDATE(optlen);
    STORM_VALIDATE_END;

    SErrSetLastError(ERROR_INVALID_PARAMETER);

    switch (optname) {
    case 1:
        if (*optlen < 4) return 0;
        *static_cast<int32_t*>(optval) = g_opt.serrleaksilentwarning;
        *optlen = 4;
        break;
    case 2:
        if (*optlen < 4) return 0;
        *static_cast<uint32_t*>(optval) = g_opt.wavechunksize;
        *optlen = 4;
        break;
    case 3:
        if (*optlen < 4) return 0;
        *static_cast<int32_t*>(optval) = g_opt.smemleaksilentwarning;
        *optlen = 4;
        break;
    case 4:
        if (*optlen < 4) return 0;
        *static_cast<int32_t*>(optval) = g_opt.alignstreamingwavedata;
        *optlen = 4;
        break;
    case 5:
        if (*optlen < 4) return 0;
        *static_cast<int32_t*>(optval) = g_opt.echotooutputdebugstring;
        *optlen = 4;
        break;
    case 6:
        if (*optlen < 8) return 0;
        static_cast<int32_t*>(optval)[0] = s_alloccount;
        static_cast<int32_t*>(optval)[1] = s_freecount;
        *optlen = 8;
        break;
    case 7:
        if (*optlen < 4) return 0;
        *static_cast<int32_t*>(optval) = g_opt.serrsuppresslogs;
        *optlen = 4;
        break;
    case 9:
        if (*optlen < 4) return 0;
        *static_cast<int32_t*>(optval) = g_opt.crcenabled;
        *optlen = 4;
        break;
    case 10:
        if (*optlen < 4) return 0;
        *static_cast<int32_t*>(optval) = g_opt.orderedprintfenabled;
        *optlen = 4;
        break;
    case 11:
        if (*optlen < 4) return 0;
        *static_cast<int32_t*>(optval) = g_opt.sregunicode;
        *optlen = 4;
        break;
    case 12:
        if (*optlen < 4) return 0;
        *static_cast<int32_t*>(optval) = g_opt.noreaderrordialog;
        *optlen = 4;
        break;
    case 13:
        if (*optlen < 4) return 0;
        *static_cast<int32_t*>(optval) = g_opt.assertlogonly;
        *optlen = 4;
        break;
#if !defined(WHOA_BAD_STORM_OPTION_ALWAYS_SUCCEEDS)
    default:
        return 0;
#endif
    }
    SErrSetLastError(ERROR_SUCCESS);
    return 1;
}
