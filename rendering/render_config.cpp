#include <string>
#include <iostream>
#include <cmath>

#include <directx/d3d12.h>
#include <directx/dxgi1_6.h>
#include <bullet/btBulletDynamicsCommon.h>

static const std::string RENDER_ENGINE  = "DirectX";
static const std::string PHYSICS_ENGINE = "BulletPhysics";
static const std::string SHADING_TYPE   = "PBR";
static const std::string TEXTURE_TYPE   = "PBR";

struct UVMap { float u, v; };
struct XYMap { float x, y; };
struct XZMap { float x, z; };

UVMap map_uv(float u, float v) { return { u, v }; }
XYMap map_xy(float x, float y) { return { x, y }; }
XZMap map_xz(float x, float z) { return { x, z }; }

void print_config() {
    std::cout << "Render Engine  : " << RENDER_ENGINE  << "\n";
    std::cout << "Physics Engine : " << PHYSICS_ENGINE << "\n";
    std::cout << "Shading Type   : " << SHADING_TYPE   << "\n";
    std::cout << "Texture Type   : " << TEXTURE_TYPE   << "\n";
}
