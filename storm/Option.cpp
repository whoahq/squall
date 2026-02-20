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
    case STORM_OPTION_SERR_LEAK_SILENT_WARN:
        if (optlen != sizeof(g_opt.serrleaksilentwarning)) return 0;
        g_opt.serrleaksilentwarning = *static_cast<const int32_t*>(optval);
        break;
    case STORM_OPTION_WAV_CHUNK_SIZE:
        if (optlen != sizeof(g_opt.wavechunksize)) return 0;

        val = *static_cast<const uint32_t*>(optval);
        if (!val || (val ^ (val - 1)) != 2 * val - 1) {
            return 0;
        }
        g_opt.wavechunksize = val;
        break;
    case STORM_OPTION_SMEM_LEAK_SILENT_WARN:
        if (optlen != sizeof(g_opt.smemleaksilentwarning)) return 0;
        g_opt.smemleaksilentwarning = *static_cast<const int32_t*>(optval);
        break;
    case STORM_OPTION_WAV_STREAMING_ALIGNED:
        if (optlen != sizeof(g_opt.alignstreamingwavedata)) return 0;
        g_opt.alignstreamingwavedata = *static_cast<const int32_t*>(optval);
        break;
    case STORM_OPTION_ECHO_OUTPUT_DEBUG_STR:
        if (optlen != sizeof(g_opt.echotooutputdebugstring)) return 0;
        g_opt.echotooutputdebugstring = *static_cast<const int32_t*>(optval);
        break;
    case STORM_OPTION_SMEM_COUNTS:
        if (optlen != sizeof(s_alloccount) + sizeof(s_freecount)) return 0;
        s_alloccount = static_cast<const uint32_t*>(optval)[0];
        s_freecount  = static_cast<const uint32_t*>(optval)[1];
        break;
    case STORM_OPTION_SERR_SUPPRESS_LOGS:
        if (optlen != sizeof(g_opt.serrsuppresslogs)) return 0;
        g_opt.serrsuppresslogs = *static_cast<const int32_t*>(optval);
        break;
    case STORM_OPTION_CRC_ENABLED:
        if (optlen != sizeof(g_opt.crcenabled)) return 0;
        g_opt.crcenabled = *static_cast<const int32_t*>(optval) != 0;
        break;
    case STORM_OPTION_ORDERED_PRINTF_ENABLED:
        if (optlen != sizeof(g_opt.orderedprintfenabled)) return 0;
        g_opt.orderedprintfenabled = *static_cast<const int32_t*>(optval) != 0;
        break;
    case STORM_OPTION_SREG_UNICODE:
        if (optlen != sizeof(g_opt.sregunicode)) return 0;
        g_opt.sregunicode = *static_cast<const int32_t*>(optval) != 0;
        break;
    case STORM_OPTION_NO_READ_ERROR_DLG:
        if (optlen != sizeof(g_opt.noreaderrordialog)) return 0;
        g_opt.noreaderrordialog = *static_cast<const int32_t*>(optval) != 0;
        break;
    case STORM_OPTION_ASSERT_LOG_ONLY:
        if (optlen != sizeof(g_opt.assertlogonly)) return 0;
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
    case STORM_OPTION_SERR_LEAK_SILENT_WARN:
        if (*optlen < sizeof(g_opt.serrleaksilentwarning)) return 0;
        *static_cast<int32_t*>(optval) = g_opt.serrleaksilentwarning;
        *optlen = sizeof(g_opt.serrleaksilentwarning);
        break;
    case STORM_OPTION_WAV_CHUNK_SIZE:
        if (*optlen < sizeof(g_opt.wavechunksize)) return 0;
        *static_cast<uint32_t*>(optval) = g_opt.wavechunksize;
        *optlen = sizeof(g_opt.wavechunksize);
        break;
    case STORM_OPTION_SMEM_LEAK_SILENT_WARN:
        if (*optlen < sizeof(g_opt.smemleaksilentwarning)) return 0;
        *static_cast<int32_t*>(optval) = g_opt.smemleaksilentwarning;
        *optlen = sizeof(g_opt.smemleaksilentwarning);
        break;
    case STORM_OPTION_WAV_STREAMING_ALIGNED:
        if (*optlen < sizeof(g_opt.alignstreamingwavedata)) return 0;
        *static_cast<int32_t*>(optval) = g_opt.alignstreamingwavedata;
        *optlen = sizeof(g_opt.alignstreamingwavedata);
        break;
    case STORM_OPTION_ECHO_OUTPUT_DEBUG_STR:
        if (*optlen < sizeof(g_opt.echotooutputdebugstring)) return 0;
        *static_cast<int32_t*>(optval) = g_opt.echotooutputdebugstring;
        *optlen = sizeof(g_opt.echotooutputdebugstring);
        break;
    case STORM_OPTION_SMEM_COUNTS:
        if (*optlen < sizeof(s_alloccount) + sizeof(s_freecount)) return 0;
        static_cast<int32_t*>(optval)[0] = s_alloccount;
        static_cast<int32_t*>(optval)[1] = s_freecount;
        *optlen = sizeof(s_alloccount) + sizeof(s_freecount);
        break;
    case STORM_OPTION_SERR_SUPPRESS_LOGS:
        if (*optlen < sizeof(g_opt.serrsuppresslogs)) return 0;
        *static_cast<int32_t*>(optval) = g_opt.serrsuppresslogs;
        *optlen = sizeof(g_opt.serrsuppresslogs);
        break;
    case STORM_OPTION_CRC_ENABLED:
        if (*optlen < sizeof(g_opt.crcenabled)) return 0;
        *static_cast<int32_t*>(optval) = g_opt.crcenabled;
        *optlen = sizeof(g_opt.crcenabled);
        break;
    case STORM_OPTION_ORDERED_PRINTF_ENABLED:
        if (*optlen < sizeof(g_opt.orderedprintfenabled)) return 0;
        *static_cast<int32_t*>(optval) = g_opt.orderedprintfenabled;
        *optlen = sizeof(g_opt.orderedprintfenabled);
        break;
    case STORM_OPTION_SREG_UNICODE:
        if (*optlen < sizeof(g_opt.sregunicode)) return 0;
        *static_cast<int32_t*>(optval) = g_opt.sregunicode;
        *optlen = sizeof(g_opt.sregunicode);
        break;
    case STORM_OPTION_NO_READ_ERROR_DLG:
        if (*optlen < sizeof(g_opt.noreaderrordialog)) return 0;
        *static_cast<int32_t*>(optval) = g_opt.noreaderrordialog;
        *optlen = sizeof(g_opt.noreaderrordialog);
        break;
    case STORM_OPTION_ASSERT_LOG_ONLY:
        if (*optlen < sizeof(g_opt.assertlogonly)) return 0;
        *static_cast<int32_t*>(optval) = g_opt.assertlogonly;
        *optlen = sizeof(g_opt.assertlogonly);
        break;
#if !defined(WHOA_BAD_STORM_OPTION_ALWAYS_SUCCEEDS)
    default:
        return 0;
#endif
    }
    SErrSetLastError(ERROR_SUCCESS);
    return 1;
}
