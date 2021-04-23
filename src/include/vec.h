#pragma once

struct vec3
{
    float x, y, z;

    static const float modulus(const vec3& vec);

    static vec3 normalize(const vec3& vec);

    static vec3 cross(const vec3& v1, const vec3& v2);

    vec3 operator+(const vec3& other)
    {
        return {x + other.x, y + other.y, z + other.z};
    }

    void operator+=(const vec3& other)
    {
        *this = *this + other;
    }

    vec3 operator-(const vec3& other)
    {
        return {x - other.x, y - other.y, z - other.z};
    }

    void operator-=(const vec3& other)
    {
        *this = *this - other;
    }

    vec3 operator*(const vec3& other)
    {
        return {x * other.x, y * other.y, z * other.z};
    }

    vec3 operator*(const float val)
    {
        return {x * val, y * val, z * val};
    }

    vec3 operator/(float val)
    {
        return {x / val, y / val, z / val};
    }
};