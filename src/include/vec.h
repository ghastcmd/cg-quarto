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

struct vec2
{
    float x, y;

    static const float modulus(const vec2& vec);

    static vec2 normalize(const vec2& vec);

    static vec2 cross(const vec2& v1, const vec2& v2);

    vec2 operator+(const vec2& other)
    {
        return {x + other.x, y + other.y};
    }

    void operator+=(const vec2& other)
    {
        *this = *this + other;
    }

    vec2 operator-(const vec2& other)
    {
        return {x - other.x, y - other.y};
    }

    void operator-=(const vec2& other)
    {
        *this = *this - other;
    }

    vec2 operator*(const vec2& other)
    {
        return {x * other.x, y * other.y};
    }

    vec2 operator*(const float val)
    {
        return {x * val, y * val};
    }

    vec2 operator/(float val)
    {
        return {x / val, y / val};
    }   
};