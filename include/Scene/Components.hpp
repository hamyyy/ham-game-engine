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

    Index()             = default;
    Index(const Index&) = default;

    Index(const uint32_t& s_index) :
        index(s_index){};

    operator uint32_t&() { return index; }
    operator const uint32_t&() const { return index; }
};

struct Transform
{

    Vector3 position = {0.f, 0.f, 0.f};      // x y z position
    Deg3            rotation = Deg3(Math::ZeroInit); // Euler angles are stroed in degrees
    Vector3 scale    = {1.f, 1.f, 1.f};      // x y z scale

    Matrix4 ToMatrix()
    {

        auto translationMat = Matrix4::translation(position);

        auto rotationMat =
            Matrix4::rotationY(rotation.y()) *
            Matrix4::rotationZ(rotation.z()) *
            Matrix4::rotationX(rotation.x());

        auto scaleMat = Matrix4::scaling(scale);

        return translationMat * rotationMat * scaleMat;
    }

    void SetFromMatrix(Float* matrix)
    {
        auto mat = Matrix4::from(matrix);

        position = mat.translation();
        rotation = Deg3(Math::Quaternion<Float>::fromMatrix(mat.rotation()).toEuler());
        scale    = mat.scaling();
    }

    Vector3 GetForward()
    {
        return ToMatrix().transformVector(Vector3::zAxis(-1)).normalized();
    }

    Vector3 GetUp()
    {
        return ToMatrix().transformVector(Vector3::yAxis()).normalized();
    }

    Vector3 GetRight()
    {
        return ToMatrix().transformVector(Vector3::xAxis()).normalized();
    }


    Transform()                 = default;
    Transform(const Transform&) = default;
    Transform(Vector3 pos) :
        position(pos) {}

    // Transform(const Matrix4& s_transform) :
    //     SceneGraph::TranslationRotationScalingTransformation3D()
    // {
    //     this->resetTransformation();
    //     this->scale(s_transform.scaling());
    //     this->rotate(Math::Quaternion<Float>::fromMatrix(s_transform.rotation()));
    //     this->translate(s_transform.translation());
    // };
};

// struct Mesh
// {
//     GL::Buffer    vertexBuffer;
//     GL::Buffer    indexBuffer;
//     MeshIndexType indexType;
//     UnsignedInt   indexCount;

//     Mesh()            = default;
//     Mesh(const Mesh&) = default;

//     Mesh(Corrade::Containers::ArrayView<const void> s_vertexBuffer, Corrade::Containers::ArrayView<const void> s_indexBuffer, MeshIndexType s_indexType = MeshIndexType::UnsignedInt) :
//         indexType(s_indexType)
//     {
//         vertexBuffer.setData(s_vertexBuffer);
//         indexBuffer.setData(s_indexBuffer);
//         indexCount = (UnsignedInt)s_indexBuffer.size();
//     }
// };

// using Material = Shaders::PhongGL;
struct ShaderMaterial
{
    Shaders::PhongGL shader;
    Color4                   color;

    ShaderMaterial(Shaders::PhongGL::Flags flags = {}, UnsignedInt lightCount = 1) :
        shader(flags, lightCount),
        color(1.f, 1.f, 1.f, 1.f) {}
};

struct MeshVisualizerShaderMaterial
{
    Shaders::MeshVisualizerGL3D shader;
    Color4                      color;
    Color4                      wireframeColor;
    float                       wireframeWidth;

    MeshVisualizerShaderMaterial(Shaders::MeshVisualizerGL3D::Flags flags = Shaders::MeshVisualizerGL3D::Flag::Wireframe | Shaders::MeshVisualizer3D::Flag::NoGeometryShader) :
        shader(flags),
        color(1.f, 1.f, 1.f, 0.f),
        wireframeColor(1.f, 1.f, 1.f, 1.f),
        wireframeWidth(1.f) {}
};

struct Mesh
{
    GL::Mesh mesh;

    Mesh(GL::MeshPrimitive primitive = GL::MeshPrimitive::Triangles) :
        mesh(primitive) {}

    operator GL::Mesh&() { return mesh; }
};

struct DebugMesh
{
    GL::Mesh mesh;

    DebugMesh(GL::MeshPrimitive primitive = GL::MeshPrimitive::Triangles) :
        mesh(primitive) {}

    operator GL::Mesh&() { return mesh; }
};

struct Light
{
    Vector3 color;
    Float   intensity;

    Light()             = default;
    Light(const Light&) = default;

    Light(const Vector3& s_color,
          const Float&   s_intensity) :
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

    Rad     fov;
    Float   aspectRatio;
    Float   nearPlane;
    Float   farPlane;
    Vector2 size;
    CameraType      type;

    Camera()              = default;
    Camera(const Camera&) = default;

    // Perspective Camera
    Camera(const Rad&   s_fov,
           const Float& s_aspectRatio,
           const Float& s_near,
           const Float& s_far) :
        fov(s_fov),
        aspectRatio(s_aspectRatio),
        nearPlane(s_near),
        farPlane(s_far),
        type(CameraType::Perspective){};

    // orthographic camera
    Camera(const Vector2& s_size,
           const Float&   s_near,
           const Float&   s_far) :
        size(s_size),
        nearPlane(s_near),
        farPlane(s_far),
        type(CameraType::Orthographic){};

    Matrix4 getProjectionMatrix()
    {
        switch (type)
        {
            case CameraType::Perspective:
                return Matrix4::perspectiveProjection(fov, aspectRatio, nearPlane, farPlane);
            case CameraType::Orthographic:
                return Matrix4::orthographicProjection(size, nearPlane, farPlane);
            default:
                break;
        }
        return Matrix4{};
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
    Entity      camera;

    GL::Framebuffer  frameBuffer;
    GL::Texture2D    colorBuffer;
    GL::Renderbuffer depthBuffer;

    // GLWindow(const GLWindow&) = default;

    GLWindow(std::string s_title, Entity s_camera) :
        title(s_title), size(512, 512), frameBuffer({{}, {}}), camera(s_camera) {}
};

struct Gizmo
{
    std::string         path;
    GL::Texture2D*      image     = nullptr;
    bool                isVisible = true;
    ImageView2D imageData;

    Gizmo()             = default;
    Gizmo(const Gizmo&) = default;

    Gizmo(const std::string s_path, Vector2i size) :
        path(s_path), imageData(PixelFormat::RGBA8Unorm, size){};
};

} // namespace Gui

} // namespace Ham::Component
