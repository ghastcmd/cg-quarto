#include "pch.h"
#include "light.hpp"

clight::clight(int lpos, vec4 pos, vec4 dir, vec4 amb, vec4 dif, vec4 spec, float cut, float exp, type tp)
{
    position[0] = pos.x;
    position[1] = pos.y;
    position[2] = pos.z;
    position[3] = pos.w;
    direction[0] = dir.x;
    direction[1] = dir.y;
    direction[2] = dir.z;
    direction[3] = dir.w;
    ambient[0] = amb.x;
    ambient[1] = amb.y;
    ambient[2] = amb.z;
    ambient[3] = amb.w;
    diffuse[0] = dif.x;
    diffuse[1] = dif.y;
    diffuse[2] = dif.z;
    diffuse[3] = dif.w;
    specular[0] = spec.x;
    specular[1] = spec.y;
    specular[2] = spec.z;
    specular[3] = spec.w;

    cutoff = cut;
    exponent = exp;

    light = GL_LIGHT0 + lpos;
    ltype = tp;
}

clight::clight(int lpos, float pos[3], float dir[3], float amb[3], float dif[3], float spec[3], float cut, float exp, type tp)
{
    for (int i = 0; i < 3; ++i)
    {
        position[i] = pos[i];
        direction[i] = dir[i];
        ambient[i] = amb[i];
        diffuse[i] = dif[i];
        specular[i] = spec[i];
    }

    cutoff = cut;
    exponent = exp;

    light = GL_LIGHT0 + lpos;
    ltype = tp;
}

void clight::apply_color() const
{
    glLightfv(light, GL_AMBIENT, ambient);
    glLightfv(light, GL_DIFFUSE, diffuse);
    glLightfv(light, GL_SPECULAR, specular);
}

void clight::dist_position() const
{
    glLightfv(light, GL_POSITION, position);
    if (ltype == type::spot_light)
    {
        glLightfv(light, GL_SPOT_DIRECTION, direction);
        glLightf(light, GL_SPOT_CUTOFF, cutoff);
        glLightf(light, GL_SPOT_EXPONENT, exponent);
    }
}

void clight::enable() const
{
    glEnable(light);
}

void clight::disable() const
{
    glDisable(light);
}