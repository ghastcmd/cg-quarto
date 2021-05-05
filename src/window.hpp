#pragma once

#include "vec.hpp"

struct window {
    window() = default;
    void init(
        int& argc, char **&argv, const char *window_name, size_t width, size_t height
    );

    void set_perspective_values(float fov, float min_dist, float max_dist);

    void set_dimensions_values(int width, int height);

    void run_perspective();

    void set_display_func(void (*fptr)(void));

    void run();

    const char *get_name() { return m_wname; }

    const char *m_wname;
    unsigned int m_width, m_height;
    float m_fov = 75.0f;
    float m_min_dist = 0.001f, m_max_dist = 1000.0f;
    bool m_setted_display = false, m_init = false;
};

struct camera
{
    vec3 pos;
    vec3 front;
    vec3 up;

    float yaw, pitch, roll;

    camera(vec3 pos, vec3 front, vec3 up, vec3 angle)
        : pos(pos), front(front), up(up)
    {
        yaw = angle.x, pitch = angle.y, roll = angle.z;
    }

    void look_at();
};