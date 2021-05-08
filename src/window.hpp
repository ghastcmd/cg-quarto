#pragma once

#include "vec.hpp"

struct window {
    window() = default;
    void init(
        int& argc, char **&argv, const char *window_name, size_t width, size_t height
    );

    void set_perspective_values(float fov, float min_dist, float max_dist);

    void run_perspective() const;

    void set_dimensions_values(int width, int height);

    void set_display_func(void (*fptr)(void));

    void run() const;

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

    float yaw = -90.0f, pitch = 0.0f, roll = 0.0f;

    camera(vec3 pos, vec3 front, vec3 up)
        : pos(pos), front(front), up(up)
    {

    }

    void center_camera_angle(window wnd)
    {
        prevx = wnd.m_width / 2;
        prevy = wnd.m_height / 2;
    }

    vec3 side_vector()
    {
        return vec3::normalize(vec3::cross(front, up));
    }

    void look_at();

    void motion(int x, int y, float mouse_sensitivity);
    void motion_capped(int x, int y, float mouse_sensitivity, vec2 horizontal_cap, vec2 vertical_cap);

private:
    float prevx, prevy;
};