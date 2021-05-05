#include "pch.h"
#include "window.hpp"

window::window(
    int& argc, char ** argv, const char *window_name,
    size_t width = 700, size_t height = 700
)
    : m_wname(window_name), m_width(width), m_height(height)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow(m_wname);

    run_perspective();
}

void window::set_perspective_values(float fov, float min_dist, float max_dist)
{
    m_fov = fov, m_min_dist = min_dist, m_max_dist = max_dist;
}

void window::set_dimensions_values(int width, int height)
{
    m_width = width, m_height = height;
}

void window::run_perspective()
{
    glViewport(0, 0, (GLsizei)m_width, (GLsizei)m_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(m_fov, (float)m_width / (float)m_height, m_min_dist, m_max_dist);
}

void window::set_display_func(void (*fptr)(void))
{
    m_setted_display = true;
    glutDisplayFunc(fptr);
    // glutReshapeFunc(*(void(*)(int, int))&_reshape_callback);
}

void window::run()
{
    if (!m_setted_display)
    {
        puts("Need to set a display function.");
        return;
    }
    glutMainLoop();
}
