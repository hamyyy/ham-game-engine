#pragma once

#ifdef HAM_ENABLE_ASSERTS

	// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
	// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
	#define HAM_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { HAM##type##ERROR(msg, __VA_ARGS__); HAM_DEBUGBREAK(); } }
	#define HAM_INTERNAL_ASSERT_WITH_MSG(type, check, ...) HAM_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define HAM_INTERNAL_ASSERT_NO_MSG(type, check) HAM_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", HAM_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define HAM_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define HAM_INTERNAL_ASSERT_GET_MACRO(...) HAM_EXPAND_MACRO( HAM_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, HAM_INTERNAL_ASSERT_WITH_MSG, HAM_INTERNAL_ASSERT_NO_MSG) )

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define HAM_ASSERT(...) HAM_EXPAND_MACRO( HAM_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define HAM_CORE_ASSERT(...) HAM_EXPAND_MACRO( HAM_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define HAM_ASSERT(...)
	#define HAM_CORE_ASSERT(...)
#endif