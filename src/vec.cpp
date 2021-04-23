#include "vec.h"
#include "pch.h"

const float vec3::modulus(const vec3& vec)
{
    return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

vec3 vec3::normalize(const vec3& vec)
{
    const float val = modulus(vec);
    return {vec.x / val, vec.y / val, vec.z / val};
}

vec3 vec3::cross(const vec3& v1, const vec3& v2)
{
    return {v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x};
}