#pragma once
#include "vec.hpp"

struct clight
{
    enum class type
    {
        point_light,
        spot_light
    };

    float position[4];
    float direction[4];
    float ambient[4];
    float diffuse[4];
    float specular[4];

    int light;

    float cutoff;
    float exponent;

    type ltype;

    clight() = default;

    clight(int lpos, vec4 pos, vec4 dir, vec4 amb, vec4 dif, vec4 spec, float cut, float exp, type tp);

    clight(int lpos, float pos[3], float dir[3], float amb[3], float dif[3], float spec[3], float cut, float exp, type tp);

    void apply_color() const;

    void dist_position() const;

    void enable() const;
    void disable() const;
};