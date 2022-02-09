#pragma once

// standard library
#include <vector>
#include <string>
#include <map>
#include <filesystem>
#include <memory>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <cmath>

// imgui
#include <imgui.h>
#include <ImGuizmo.h>

// entt
#include <entt/entt.hpp>

// lunaSVG
#include <lunasvg.h>

// Magnum Platform
#include <Magnum/Platform/GlfwApplication.h>

// Magnum Math
#include <Magnum/Math/Math.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Vector3.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/Math/Quaternion.h>
#include <Magnum/Math/Constants.h>

// Magnum GL
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Renderbuffer.h>
#include <Magnum/GL/Framebuffer.h>
#include <Magnum/GL/TextureFormat.h>
#include <Magnum/GL/RenderbufferFormat.h>

// Magnum Shaders
#include <Magnum/Shaders/PhongGL.h>
#include <Magnum/Shaders/MeshVisualizerGL.h>

// Magnum Primitives
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Primitives/Line.h>
#include <Magnum/Primitives/Circle.h>
#include <Magnum/Primitives/Square.h>

// Magnum SceneGraph
#include <Magnum/SceneGraph/SceneGraph.h>
#include <Magnum/SceneGraph/Object.h>
#include <Magnum/SceneGraph/TranslationRotationScalingTransformation3D.h>
#include <Magnum/SceneGraph/MatrixTransformation3D.h>

// Magnum MeshTools
#include <Magnum/MeshTools/CompressIndices.h>
#include <Magnum/MeshTools/Interleave.h>

// Magnum Trade
#include <Magnum/Trade/MeshData.h>
#include <Magnum/Trade/ImageData.h>

// Magnum ImGui Integration
#include <Magnum/ImGuiIntegration/Context.hpp>
#include <Magnum/ImGuiIntegration/Widgets.h>

// Magnum image processing
#include <Magnum/PixelFormat.h>
#include <Magnum/ImageView.h>

// Magnum Containers
#include <Magnum/Array.h>

// Magnum Core Features
#include <Magnum/Timeline.h>