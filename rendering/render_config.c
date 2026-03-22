#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

static const char* RENDER_ENGINE  = "DirectX";
static const char* PHYSICS_ENGINE = "BulletPhysics";
static const char* SHADING_TYPE   = "PBR";
static const char* TEXTURE_TYPE   = "PBR";

typedef struct { float u; float v; } UVMap;
typedef struct { float x; float y; } XYMap;
typedef struct { float x; float z; } XZMap;

UVMap map_uv(float u, float v) { UVMap m; m.u = u; m.v = v; return m; }
XYMap map_xy(float x, float y) { XYMap m; m.x = x; m.y = y; return m; }
XZMap map_xz(float x, float z) { XZMap m; m.x = x; m.z = z; return m; }

void print_config(void) {
    printf("Render Engine  : %s\n", RENDER_ENGINE);
    printf("Physics Engine : %s\n", PHYSICS_ENGINE);
    printf("Shading Type   : %s\n", SHADING_TYPE);
    printf("Texture Type   : %s\n", TEXTURE_TYPE);
}
