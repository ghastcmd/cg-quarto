#include "pch.h"
#include "vec.hpp"

const float vec4::modulus(const vec4& vec)
{
    return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
}

vec4 vec4::normalize(const vec4& vec)
{
    const float val = modulus(vec);
    return {vec.x / val, vec.y / val, vec.z / val};
}

vec4 vec4::cross(const vec4& v1, const vec4& v2)
{
    const vec3 _ret = vec3::cross({v1.x, v1.y, v1.z}, {v2.x, v2.y, v2.z});
    return {_ret.x, _ret.y, _ret.z, 1};
}

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

const float vec2::modulus(const vec2& vec)
{
    return sqrt(vec.x * vec.x + vec.y * vec.y);
}

vec2 vec2::normalize(const vec2& vec)
{
    const float val = modulus(vec);
    return {vec.x / val, vec.y / val};
}
