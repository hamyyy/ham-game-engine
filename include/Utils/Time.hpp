#pragma once

namespace Ham
{
namespace Time
{

inline std::chrono::steady_clock::time_point StartTime;
inline std::chrono::steady_clock::time_point PreviousFrameTime;
inline float                                DeltaTime = 0.0; // seconds

inline void Initialize()
{
    StartTime         = std::chrono::high_resolution_clock::now();
    PreviousFrameTime = StartTime;
    DeltaTime         = 0;
}

inline void NextFrame()
{
    auto now          = std::chrono::high_resolution_clock::now();
    auto duration     = (float)std::chrono::duration_cast<std::chrono::microseconds>(now - PreviousFrameTime).count() / 1000000.0f;
    DeltaTime         = duration;
    PreviousFrameTime = now;
}

inline float GetPreviousFrameTime()
{
    return (float)std::chrono::duration_cast<std::chrono::microseconds>(PreviousFrameTime - StartTime).count() / 1000000.0f;
}

} // namespace Time
} // namespace Ham
