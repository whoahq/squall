#ifndef STORM_ERROR_MACROS_HPP
#define STORM_ERROR_MACROS_HPP

// STORM_APP_FATAL
#if !defined(NDEBUG) || defined(WHOA_ASSERTIONS_ENABLED)
// Debug Build + Release Assertions Enabled Build
#define STORM_APP_FATAL(...)                                                                        \
            SErrPrepareAppFatal(__FILE__, __LINE__);                                                \
            SErrDisplayAppFatal(__VA_ARGS__)
#else
// Release Build
#define STORM_APP_FATAL(...)                                                                        \
            SErrDisplayAppFatal(__VA_ARGS__)
#endif

// STORM_ASSERT
#if !defined(NDEBUG) || defined(WHOA_ASSERTIONS_ENABLED)
// Debug Build + Release Assertions Enabled Build
#define STORM_ASSERT(x)                                                                             \
            if (!(x)) {                                                                             \
                SErrDisplayError(STORM_ERROR_ASSERTION, __FILE__, __LINE__, #x, 0, 1, 0x11111111);  \
            }
#else
// Release Build
#define STORM_ASSERT(x)
#endif

// STORM_VALIDATE_ASSERT
#if !defined(NDEBUG)
// Debug Build
#define STORM_VALIDATE_ASSERT(x)                                                                    \
            STORM_ASSERT(x)
#elif defined(WHOA_ASSERTIONS_ENABLED)
// Release Assertions Enabled Build
#define STORM_VALIDATE_ASSERT(x)                                                                    \
            if (!(x)) {                                                                             \
                STORM_APP_FATAL(#x);                                                                \
            }
#else
// Release Build
inline void NullAppFatal(const char* str) {};
#define STORM_VALIDATE_ASSERT(x)                                                                    \
            if (!(x)) {                                                                             \
                NullAppFatal(#x);                                                                   \
            }
#endif

// STORM_VALIDATE
#if !defined(NDEBUG)
// Debug Build
#define STORM_VALIDATE_BEGIN
#define STORM_VALIDATE(x)                                                                           \
            STORM_VALIDATE_ASSERT(x)
#define STORM_VALIDATE_END
#define STORM_VALIDATE_END_VOID
#else
// Release Build + Release Assertions Enabled Build
#define STORM_VALIDATE_BEGIN                                                                        \
            {                                                                                       \
                bool __storm_result = true
#define STORM_VALIDATE(x)                                                                           \
                __storm_result &= !!(x);                                                            \
                STORM_VALIDATE_ASSERT(x)
#define STORM_VALIDATE_END                                                                          \
                if (!__storm_result) {                                                              \
                    SErrSetLastError(ERROR_INVALID_PARAMETER);                                      \
                    return 0;                                                                       \
                }                                                                                   \
            }
#define STORM_VALIDATE_END_VOID                                                                     \
                if (!__storm_result) {                                                              \
                    SErrSetLastError(ERROR_INVALID_PARAMETER);                                      \
                    return;                                                                         \
                }                                                                                   \
            }
#endif

#endif
