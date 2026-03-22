#include <stdio.h>
#include "shadow.h"

/* -----------------------------------------------------------------------
   Vec3 helpers
   ----------------------------------------------------------------------- */

Vec3 vec3_add(Vec3 a, Vec3 b) {
    Vec3 r; r.x = a.x + b.x; r.y = a.y + b.y; r.z = a.z + b.z; return r;
}

Vec3 vec3_sub(Vec3 a, Vec3 b) {
    Vec3 r; r.x = a.x - b.x; r.y = a.y - b.y; r.z = a.z - b.z; return r;
}

Vec3 vec3_scale(Vec3 v, float s) {
    Vec3 r; r.x = v.x * s; r.y = v.y * s; r.z = v.z * s; return r;
}

float vec3_length(Vec3 v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vec3 vec3_normalize(Vec3 v) {
    float len = vec3_length(v);
    if (len < 1e-6f) { Vec3 z = {0, 0, 0}; return z; }
    return vec3_scale(v, 1.0f / len);
}

float vec3_dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 vec3_reflect(Vec3 incident, Vec3 normal) {
    float d = 2.0f * vec3_dot(incident, normal);
    return vec3_sub(incident, vec3_scale(normal, d));
}

/* -----------------------------------------------------------------------
   Object contact test — objects are touching when the distance between
   their centres is less than or equal to the sum of their radii.
   ----------------------------------------------------------------------- */

int objects_are_touching(SphereObject a, SphereObject b) {
    Vec3  delta    = vec3_sub(b.position, a.position);
    float dist     = vec3_length(delta);
    float combined = a.radius + b.radius;
    return (dist <= combined);
}

/* -----------------------------------------------------------------------
   Ray–sphere intersection using the analytic formula.
   Tests every object in the scene and returns the closest hit.
   ----------------------------------------------------------------------- */

RayHit ray_trace(Ray ray, SphereObject* objects, int object_count) {
    RayHit closest;
    closest.did_hit  = 0;
    closest.distance = 1e30f;

    for (int i = 0; i < object_count; i++) {
        Vec3  oc = vec3_sub(ray.origin, objects[i].position);
        float a  = vec3_dot(ray.direction, ray.direction);
        float b  = 2.0f * vec3_dot(oc, ray.direction);
        float c  = vec3_dot(oc, oc) - objects[i].radius * objects[i].radius;
        float d  = b * b - 4.0f * a * c;

        if (d < 0.0f) continue;

        float t = (-b - sqrtf(d)) / (2.0f * a);
        if (t < 1e-4f) t = (-b + sqrtf(d)) / (2.0f * a);
        if (t < 1e-4f) continue;

        if (t < closest.distance) {
            closest.distance   = t;
            closest.hit_point  = vec3_add(ray.origin, vec3_scale(ray.direction, t));
            closest.hit_normal = vec3_normalize(vec3_sub(closest.hit_point, objects[i].position));
            closest.did_hit    = 1;
        }
    }

    return closest;
}

/* -----------------------------------------------------------------------
   Shadow softness — computes the penumbra factor using the angle between
   the hit normal and the light direction, weighted by the settings softness.
   Returns a value in [0, 1] where 0 = sharp edge, 1 = fully soft.
   ----------------------------------------------------------------------- */

float shadow_softness_penumbra(RayHit hit, ShadowSettings settings, GlobalLight light) {
    if (!hit.did_hit) return 0.0f;

    Vec3  to_light = vec3_normalize(light.direction);
    float ndotl    = vec3_dot(hit.hit_normal, to_light);
    float penumbra = 1.0f - fmaxf(0.0f, fminf(1.0f, ndotl));
    return penumbra * settings.softness;
}

/* -----------------------------------------------------------------------
   Screen-space specularity — uses the view direction, fragment normal, and
   global light to compute a Blinn-Phong specular term clamped and scaled
   by the shadow settings specularity value.
   ----------------------------------------------------------------------- */

float screen_space_specularity(ScreenSpaceFragment frag, GlobalLight light, ShadowSettings settings) {
    Vec3 light_dir  = vec3_normalize(light.direction);
    Vec3 half_vec   = vec3_normalize(vec3_add(light_dir, frag.view_dir));
    float ndoth     = fmaxf(0.0f, vec3_dot(frag.frag_normal, half_vec));
    float spec_pow  = 32.0f * (1.0f - settings.softness);
    float specular  = powf(ndoth, fmaxf(1.0f, spec_pow));
    return specular * settings.specularity * light.intensity;
}

/* -----------------------------------------------------------------------
   Shadow rotation from time — rotates the light direction around the Y axis
   based on elapsed time, simulating a day/night cycle or moving light source.
   Returns the rotation angle in radians.
   ----------------------------------------------------------------------- */

float shadow_rotation_from_time(float time_seconds) {
    float cycle = 120.0f;
    return (fmodf(time_seconds, cycle) / cycle) * 2.0f * SHADOW_PI;
}

/* -----------------------------------------------------------------------
   Main shadow computation — called when two objects are touching.
   1. Checks contact between the caster and every other object.
   2. Fires a ray from the contact point toward the (time-rotated) light.
   3. Applies softness, opacity, color, and screen-space specularity.
   Returns a ShadowResult containing the blended final shadow colour,
   opacity, specular contribution, and whether the point is in shadow.
   ----------------------------------------------------------------------- */

ShadowResult compute_shadow(
    SphereObject*        objects,
    int                  object_count,
    int                  object_index,
    ShadowSettings       settings,
    GlobalLight          light,
    ScreenSpaceFragment  frag,
    float                time_seconds
) {
    ShadowResult result;
    result.final_color    = settings.color;
    result.final_opacity  = 0.0f;
    result.specular_amount = 0.0f;
    result.in_shadow       = 0;

    if (!settings.enabled) return result;

    float angle    = shadow_rotation_from_time(time_seconds);
    float cos_a    = cosf(angle);
    float sin_a    = sinf(angle);

    Vec3 rotated_light;
    rotated_light.x = light.direction.x * cos_a - light.direction.z * sin_a;
    rotated_light.y = light.direction.y;
    rotated_light.z = light.direction.x * sin_a + light.direction.z * cos_a;
    rotated_light   = vec3_normalize(rotated_light);

    GlobalLight timed_light = light;
    timed_light.direction   = rotated_light;

    SphereObject caster = objects[object_index];

    for (int i = 0; i < object_count; i++) {
        if (i == object_index) continue;
        if (!objects_are_touching(caster, objects[i])) continue;

        Vec3 contact = vec3_add(
            caster.position,
            vec3_scale(vec3_normalize(vec3_sub(objects[i].position, caster.position)), caster.radius)
        );

        Ray shadow_ray;
        shadow_ray.origin    = vec3_add(contact, vec3_scale(rotated_light, 1e-3f));
        shadow_ray.direction = vec3_scale(rotated_light, -1.0f);

        RayHit hit = ray_trace(shadow_ray, objects, object_count);

        if (hit.did_hit) {
            result.in_shadow = 1;

            float penumbra     = shadow_softness_penumbra(hit, settings, timed_light);
            float base_opacity = settings.opacity * (1.0f - penumbra);

            float spec = screen_space_specularity(frag, timed_light, settings);

            Vec3 spec_tint;
            spec_tint.x = timed_light.color.x * spec;
            spec_tint.y = timed_light.color.y * spec;
            spec_tint.z = timed_light.color.z * spec;

            result.final_color.x  = fminf(1.0f, settings.color.x + spec_tint.x);
            result.final_color.y  = fminf(1.0f, settings.color.y + spec_tint.y);
            result.final_color.z  = fminf(1.0f, settings.color.z + spec_tint.z);
            result.final_opacity  = fminf(1.0f, base_opacity);
            result.specular_amount = spec;
            break;
        }
    }

    return result;
}
