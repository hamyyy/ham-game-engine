#pragma once

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/ostream_sink.h>

namespace Ham
{


class Log
{
public:
    inline static void Initialize()
    {
        s_LoggerStream = {};

        std::vector<spdlog::sink_ptr> logSinks;
        logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Ham.log", true));
        logSinks.emplace_back(std::make_shared<spdlog::sinks::ostream_sink_mt>(s_LoggerStream));

        logSinks[0]->set_pattern("%^[%T] %n: %v%$");
        logSinks[1]->set_pattern("[%T] [%l] %n: %v");
        logSinks[2]->set_pattern("%^[%T] %n: %v%$");

        s_CoreLogger = std::make_shared<spdlog::logger>("HAM", begin(logSinks), end(logSinks));
        spdlog::register_logger(s_CoreLogger);
        s_CoreLogger->set_level(spdlog::level::trace);
        s_CoreLogger->flush_on(spdlog::level::trace);

        s_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
        spdlog::register_logger(s_ClientLogger);
        s_ClientLogger->set_level(spdlog::level::trace);
        s_ClientLogger->flush_on(spdlog::level::trace);
    }

    inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
    inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

    inline static std::ostringstream& GetLoggerStringStream() { return s_LoggerStream; }

    inline static void SuppressCore() { s_CoreLogger->set_level(spdlog::level::off); }

    inline static void SuppressClient() { s_ClientLogger->set_level(spdlog::level::off); }

    inline static void UnsuppressCore() { s_CoreLogger->set_level(spdlog::level::trace); }

    inline static void UnsuppressClient() { s_ClientLogger->set_level(spdlog::level::trace); }

    inline static void Suppress()
    {
        s_CoreLogger->set_level(spdlog::level::off);
        s_ClientLogger->set_level(spdlog::level::off);
    }

    inline static void Unsuppress()
    {
        s_CoreLogger->set_level(spdlog::level::trace);
        s_ClientLogger->set_level(spdlog::level::trace);
    }


private:
    inline static Ref<spdlog::logger> s_CoreLogger;
    inline static Ref<spdlog::logger> s_ClientLogger;

    inline static std::ostringstream s_LoggerStream;
};

// Ref<spdlog::logger> Log::s_CoreLogger;
// Ref<spdlog::logger> Log::s_ClientLogger;

} // namespace Ham

// template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
// inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
// {
// 	return os << glm::to_string(vector);
// }

// template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
// inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
// {
// 	return os << glm::to_string(matrix);
// }

// template<typename OStream, typename T, glm::qualifier Q>
// inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
// {
// 	return os << glm::to_string(quaternion);
// }

// Core log macros
#define HAM_CORE_TRACE(...)    ::Ham::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define HAM_CORE_INFO(...)     ::Ham::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HAM_CORE_WARN(...)     ::Ham::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HAM_CORE_ERROR(...)    ::Ham::Log::GetCoreLogger()->error(__VA_ARGS__)
#define HAM_CORE_CRITICAL(...) ::Ham::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define HAM_TRACE(...)    ::Ham::Log::GetClientLogger()->trace(__VA_ARGS__)
#define HAM_INFO(...)     ::Ham::Log::GetClientLogger()->info(__VA_ARGS__)
#define HAM_WARN(...)     ::Ham::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HAM_ERROR(...)    ::Ham::Log::GetClientLogger()->error(__VA_ARGS__)
#define HAM_CRITICAL(...) ::Ham::Log::GetClientLogger()->critical(__VA_ARGS__)