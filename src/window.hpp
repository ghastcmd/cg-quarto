#pragma once

struct window {
    window() = delete;
    window(
        int& argc, char ** argv, const char *window_name,
        size_t width = 700, size_t height = 700
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
    bool m_setted_display = false;
};