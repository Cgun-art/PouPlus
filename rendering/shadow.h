#ifndef SHADOW_H
#define SHADOW_H

#include <math.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RAY_STEPS    256
#define RAY_STEP_SIZE    0.01f
#define SHADOW_PI        3.14159265358979323846f

typedef struct {
    float x, y, z;
} Vec3;

typedef struct {
    Vec3  origin;
    Vec3  direction;
} Ray;

typedef struct {
    Vec3  position;
    float radius;
} SphereObject;

typedef struct {
    float softness;
    float opacity;
    int   enabled;
    Vec3  color;
    float specularity;
} ShadowSettings;

typedef struct {
    Vec3  hit_point;
    Vec3  hit_normal;
    float distance;
    int   did_hit;
} RayHit;

typedef struct {
    Vec3  direction;
    Vec3  color;
    float intensity;
} GlobalLight;

typedef struct {
    Vec3  frag_pos;
    Vec3  frag_normal;
    Vec3  view_dir;
    float depth;
} ScreenSpaceFragment;

typedef struct {
    Vec3  final_color;
    float final_opacity;
    float specular_amount;
    int   in_shadow;
} ShadowResult;

Vec3  vec3_add(Vec3 a, Vec3 b);
Vec3  vec3_sub(Vec3 a, Vec3 b);
Vec3  vec3_scale(Vec3 v, float s);
Vec3  vec3_normalize(Vec3 v);
float vec3_dot(Vec3 a, Vec3 b);
float vec3_length(Vec3 v);
Vec3  vec3_reflect(Vec3 incident, Vec3 normal);

int   objects_are_touching(SphereObject a, SphereObject b);
RayHit ray_trace(Ray ray, SphereObject* objects, int object_count);
float shadow_softness_penumbra(RayHit hit, ShadowSettings settings, GlobalLight light);
float screen_space_specularity(ScreenSpaceFragment frag, GlobalLight light, ShadowSettings settings);
float shadow_rotation_from_time(float time_seconds);
ShadowResult compute_shadow(
    SphereObject*        objects,
    int                  object_count,
    int                  object_index,
    ShadowSettings       settings,
    GlobalLight          light,
    ScreenSpaceFragment  frag,
    float                time_seconds
);

#endif
