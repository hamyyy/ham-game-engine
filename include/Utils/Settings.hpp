#pragma once


namespace Ham
{
namespace Settings
{
inline ImGuizmo::OPERATION manipulateOperation = ImGuizmo::OPERATION::TRANSLATE;
inline float               linearSnapValue     = 0.5f;
inline float               angularSnapValue    = 45.0f;
inline float               currentSnapValue    = 0;
inline bool                useSnap             = false;




inline void ReadSettingsFromFile()
{
}
} // namespace Settings

} // namespace Ham
