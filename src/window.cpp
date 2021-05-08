#include "pch.h"
#include "window.hpp"
#include "vec.hpp"
#include "math.hpp"

void window::init(
    int& argc, char **&argv, const char *window_name, size_t width, size_t height
)
{
    if (m_init) return;
    m_init = true;
    m_wname = window_name, m_width = width, m_height = height;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
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

void window::run_perspective() const
{
    glViewport(0, 0, (GLsizei)m_width, (GLsizei)m_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(m_fov, (float)m_width / (float)m_height, m_min_dist, m_max_dist);
}

void window::set_display_func(void (*fptr)(void))
{
    if (!m_init)
    {
        puts("Need to run init function before.");
        return;
    }
    m_setted_display = true;
    glutDisplayFunc(fptr);
}

void window::run() const
{
    if (!m_setted_display)
    {
        puts("Need to set a display function.");
        return;
    }
    glutMainLoop();
}

void camera::look_at()
{
    vec3 look = pos + front;
    gluLookAt(pos.x, pos.y, pos.z, look.x, look.y, look.z, up.x, up.y, up.z);
}

void camera::motion(int x, int y, float sensitivity)
{
    float xoffset = x - prevx, yoffset = prevy - y;
    xoffset *= sensitivity, yoffset *= sensitivity;
    prevx = x, prevy = y;

    yaw += xoffset;
    pitch += yoffset;

    pitch = std::clamp(pitch, -89.0f, 89.0f); // not working on linux ??
    // pitch = pitch < -89.0f ? -89.0f : pitch > 89.0f ? 89.0f : pitch;

    vec3 direction {
        cos(radians(yaw)) * cos(radians(pitch)),
        sin(radians(pitch)),
        sin(radians(yaw)) * cos(radians(pitch))
    };

    front = vec3::normalize(direction);
}

void camera::motion_capped(int x, int y, float sensitivity, vec2 horizontal_cap, vec2 vertical_cap)
{
    float xoffset = x - prevx, yoffset = prevy - y;
    xoffset *= sensitivity, yoffset *= sensitivity;
    prevx = x, prevy = y;

    yaw += xoffset;
    pitch += yoffset;

    yaw = std::clamp(yaw, horizontal_cap.x, horizontal_cap.y);
    pitch = std::clamp(pitch, vertical_cap.x, vertical_cap.y); // not working on linux ??
    // pitch = pitch < -89.0f ? -89.0f : pitch > 89.0f ? 89.0f : pitch;

    vec3 direction {
        cos(radians(yaw)) * cos(radians(pitch)),
        sin(radians(pitch)),
        sin(radians(yaw)) * cos(radians(pitch))
    };

    front = vec3::normalize(direction);
}