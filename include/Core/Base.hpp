#pragma once

// Available Platforms
/*
        CORRADE_TARGET_APPLE
        CORRADE_TARGET_IOS
        CORRADE_TARGET_IOS_SIMULATOR
        CORRADE_TARGET_UNIX
        CORRADE_TARGET_WINDOWS
        CORRADE_TARGET_WINDOWS_RT
        CORRADE_TARGET_EMSCRIPTEN
        CORRADE_TARGET_ANDROID
*/

#ifdef _DEBUG
#    if defined(CORRADE_TARGET_WINDOWS)
#        define HAM_DEBUGBREAK() __debugbreak()
#    elif defined(CORRADE_TARGET_UNIX)
#        include <signal.h>
#        define HAM_DEBUGBREAK() raise(SIGTRAP)
#    else
#        error "Platform doesn't support debugbreak yet!"
#    endif
#    define HAM_ENABLE_ASSERTS
#else
#    define HAM_DEBUGBREAK()
#endif

#define HAM_EXPAND_MACRO(x)    x
#define HAM_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define rad_to_deg(x) ((x) * 57.295779513082320876798154814105f)
#define deg_to_rad(x) ((x) * 0.01745329251994329576923690768489f)

#define HAM_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { \
    return this->fn(std::forward<decltype(args)>(args)...);              \
}

namespace Ham
{

template <typename T>
using Scope = std::unique_ptr<T>;
template <typename T, typename... Args>
constexpr Scope<T> CreateScope(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
using Ref = std::shared_ptr<T>;
template <typename T, typename... Args>
constexpr Ref<T> CreateRef(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

} // namespace Ham