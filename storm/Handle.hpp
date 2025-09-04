#ifndef STORM_HANDLE_HPP
#define STORM_HANDLE_HPP

#define DECLARE_STORM_HANDLE(name)          \
    struct name##__ { int32_t unused; };    \
    typedef struct name##__* name

#define DECLARE_STORM_CHILD_HANDLE(name, super)             \
    struct name##__ : public super##__ { int32_t unused; }; \
    typedef struct name##__* name

#endif
