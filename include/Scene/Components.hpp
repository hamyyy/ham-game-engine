#pragma once

#include "Core/Core.hpp"

namespace Ham::Component
{

struct Tag
{
    std::string tag;

    Tag()           = default;
    Tag(const Tag&) = default;

    Tag(const std::string& s_tag) :
        tag(s_tag){};

    operator std::string&() { return tag; }
    operator const std::string&() const { return tag; }
};

struct Index
{
    uint32_t index;

    Index()           = default;
    Index(const Index&) = default;

    Index(const uint32_t& s_index) :
        index(s_index){};

    operator uint32_t&() { return index; }
    operator const uint32_t&() const { return index; }
};

struct Transform
{

    Magnum::Vector3 position = {0.f, 0.f, 0.f};      // x y z position
    Deg3            rotation = Deg3(Math::ZeroInit); // Euler angles are stroed in degrees
    Magnum::Vector3 scale    = {1.f, 1.f, 1.f};      // x y z scale

    Magnum::Matrix4 ToMatrix()
    {

        auto translationMat = Magnum::Matrix4::translation(position);

        auto rotationMat =
            Magnum::Matrix4::rotationZ(rotation.z()) *
            Magnum::Matrix4::rotationY(rotation.y()) *
            Magnum::Matrix4::rotationX(rotation.x());

        auto scaleMat = Magnum::Matrix4::scaling(scale);

        return translationMat * rotationMat * scaleMat;
    }

    void SetFromMatrix(Magnum::Float* matrix)
    {
        auto mat = Matrix4::from(matrix);

        position = mat.translation();
        rotation = Deg3(Math::Quaternion<Magnum::Float>::fromMatrix(mat.rotation()).toEuler());
        scale    = mat.scaling();
    }

    Transform()                 = default;
    Transform(const Transform&) = default;
    Transform(Magnum::Vector3 pos) :
        position(pos) {}

    // Transform(const Magnum::Matrix4& s_transform) :
    //     Magnum::SceneGraph::TranslationRotationScalingTransformation3D()
    // {
    //     this->resetTransformation();
    //     this->scale(s_transform.scaling());
    //     this->rotate(Magnum::Math::Quaternion<Magnum::Float>::fromMatrix(s_transform.rotation()));
    //     this->translate(s_transform.translation());
    // };
};

// struct Mesh
// {
//     Magnum::GL::Buffer    vertexBuffer;
//     Magnum::GL::Buffer    indexBuffer;
//     Magnum::MeshIndexType indexType;
//     Magnum::UnsignedInt   indexCount;

//     Mesh()            = default;
//     Mesh(const Mesh&) = default;

//     Mesh(Corrade::Containers::ArrayView<const void> s_vertexBuffer, Corrade::Containers::ArrayView<const void> s_indexBuffer, Magnum::MeshIndexType s_indexType = Magnum::MeshIndexType::UnsignedInt) :
//         indexType(s_indexType)
//     {
//         vertexBuffer.setData(s_vertexBuffer);
//         indexBuffer.setData(s_indexBuffer);
//         indexCount = (Magnum::UnsignedInt)s_indexBuffer.size();
//     }
// };

using Mesh = Magnum::GL::Mesh;

// using Material = Magnum::Shaders::PhongGL;
struct Material : public Magnum::Shaders::PhongGL
{
    Magnum::Color4 color;
};

struct Light
{
    Magnum::Vector3 color;
    Magnum::Float   intensity;

    Light()             = default;
    Light(const Light&) = default;

    Light(const Magnum::Vector3& s_color,
          const Magnum::Float&   s_intensity) :
        color(s_color),
        intensity(s_intensity){};
};

struct Camera
{
    enum CameraType
    {
        Perspective,
        Orthographic
    };

    Magnum::Rad     fov;
    Magnum::Float   aspectRatio;
    Magnum::Float   nearPlane;
    Magnum::Float   farPlane;
    Magnum::Vector2 size;
    CameraType      type;

    Camera()              = default;
    Camera(const Camera&) = default;

    // Perspective Camera
    Camera(const Magnum::Rad&   s_fov,
           const Magnum::Float& s_aspectRatio,
           const Magnum::Float& s_near,
           const Magnum::Float& s_far) :
        fov(s_fov),
        aspectRatio(s_aspectRatio),
        nearPlane(s_near),
        farPlane(s_far),
        type(CameraType::Perspective){};

    // orthographic camera
    Camera(const Magnum::Vector2& s_size,
           const Magnum::Float&   s_near,
           const Magnum::Float&   s_far) :
        size(s_size),
        nearPlane(s_near),
        farPlane(s_far),
        type(CameraType::Orthographic){};

    Magnum::Matrix4 getProjectionMatrix()
    {
        switch (type)
        {
            case CameraType::Perspective:
                return Magnum::Matrix4::perspectiveProjection(fov, aspectRatio, nearPlane, farPlane);
            case CameraType::Orthographic:
                return Magnum::Matrix4::orthographicProjection(size, nearPlane, farPlane);
            default:
                break;
        }
        return Magnum::Matrix4{};
    }
};

namespace Gui
{

struct ImGuiWindow
{
    bool        isOpen            = true;
    bool        isFocused         = false;
    bool        isTitlebarDragged = false;
    std::string title;

    // ImGuiWindow(const ImGuiWindow&) = default;
    ImGuiWindow(std::string s_title) :
        title(s_title) {}
};

struct GLWindow
{
    bool        isOpen            = true;
    bool        isFocused         = false;
    bool        isTitlebarDragged = false;
    Vector2i    size;
    std::string title;

    Magnum::GL::Framebuffer frameBuffer;
    GL::Texture2D           colorBuffer;
    GL::Renderbuffer        depthBuffer;

    // GLWindow(const GLWindow&) = default;
    GLWindow(std::string s_title) :
        title(s_title), size(500, 500), frameBuffer({{}, {}}) {}
};

} // namespace Gui

} // namespace Ham::Component
