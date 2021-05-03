#include "pch.h"
#include "vec.hpp"
#include "reader.hpp"

template <typename _ty>
constexpr _ty PI = (_ty)3.14159265358979323846;

float radians(float val)
{
    return val * (PI<float> / 180.0f);
}

struct camera
{
    vec3 pos;
    vec3 front;
    vec3 up;
    struct {
        float yaw, pitch, roll;
    };
};

camera cam{ {0.0f, 0.0f, 5.0f}, {0.0f, 0.0f, -3.0f}, {0.0f, 1.0f, 0.0f}, {-90.0f, 0.0f, 0.0f} };

#define dist(vec) vec.x, vec.y, vec.z

float last_frame, dt;
float fov = 75.0f;
float speed = 4.8f * 2.0f;
float mouse_sensitivity = 0.22f;

void timer(int count)
{
    float current_frame = glutGet(GLUT_ELAPSED_TIME);
    dt = current_frame - last_frame;
    last_frame = current_frame;

    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);
}

std::vector<obj_file> models;
obj_file simple;

float test_angle = 0;
float dw_angle_n_pos[] = {0.0f, 0.0f, 0.0f};
unsigned int control_index = 0;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    vec3 look = cam.pos + cam.front;
    gluLookAt(dist(cam.pos), dist(look), dist(cam.up));

    glPushMatrix();
        glScalef(2.0f, 2.0f, 2.0f);
        glTranslatef(5.0f, 0.0f, 2.0f);

        glPushMatrix(); // first door
            glTranslatef(-3.63f, 0.040f, 2.5f);
            glRotatef(dw_angle_n_pos[0], 0, 1, 0);
            glTranslatef(0.4f, 0.0f, 0.0f);
            glScalef(0.7624f, 2.058f, 0.03f);
            glutSolidCube(1.0f);
        glPopMatrix();

        glPushMatrix(); // second door
            glTranslatef(-10.0f, 0.03f, 2.427f);
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
            glRotatef(-dw_angle_n_pos[1], 0.0f, 1.0f, 0.0f);
            glTranslatef(0.4f, 0.0f, 0.0f);
            glScalef(0.7624f, 2.058f, 0.03f);
            glutSolidCube(1.0f);
        glPopMatrix();

        glPushMatrix(); // left window (imovable)
            glTranslatef(-5.745f, 0.585f, -1.66f);
            glScalef(1.55f, 1.125f, 0.03f);
            glutSolidCube(1.0f);
        glPopMatrix();

        glPushMatrix(); // right window (movable)
            glTranslatef(-4.255f - dw_angle_n_pos[2] / 65.0f, 0.585f, -1.62f);
            glScalef(1.55f, 1.125f, 0.03f);
            glutSolidCube(1.0f);
        glPopMatrix();

        glPushMatrix(); // drawing the bedroom
            glColor3f(1, 0, 0);
            glTranslatef(-5.0f, -1.0f, 0.0f);
            models[0].draw_mesh();
        glPopMatrix();
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int width, int height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, (GLfloat)width / (GLfloat)height, 0.001f, 1000.0f);
}

void motion(int x, int y)
{
    static float prevx = glutGet(GLUT_WINDOW_WIDTH) / 2, prevy = glutGet(GLUT_WINDOW_HEIGHT) / 2;
    float xoffset = x - prevx, yoffset = prevy - y;
    xoffset *= mouse_sensitivity, yoffset *= mouse_sensitivity;
    prevx = x, prevy = y;

    cam.yaw += xoffset;
    cam.pitch += yoffset;

    // cam.pitch = std::clamp(cam.pitch, -89.0f, 89.0f); // not working on linux ??
    cam.pitch = cam.pitch < -89.0f ? -89.0f : cam.pitch > 89.0f ? 89.0f : cam.pitch;

    vec3 direction {
        cos(radians(cam.yaw)) * cos(radians(cam.pitch)),
        sin(radians(cam.pitch)),
        sin(radians(cam.yaw)) * cos(radians(cam.pitch))
    };

    cam.front = vec3::normalize(direction);
}

void keyboard(unsigned char key, int x, int y)
{
    float cam_speed = speed * dt / 1000;
    float &ct_var = dw_angle_n_pos[control_index];
    switch (key)
    {
        case 'w':
            cam.pos += cam.front * cam_speed / 2.0f;
        break;
        case 's':
            cam.pos -= cam.front * cam_speed / 2.0f;
        break;
        case 'a':
            cam.pos -= vec3::normalize(vec3::cross(cam.front, cam.up)) * cam_speed;
        break;
        case 'd':
            cam.pos += vec3::normalize(vec3::cross(cam.front, cam.up)) * cam_speed;
        break;
        case ' ':
            cam.pos += vec3{0, cam_speed, 0};
        break;
        case 'b':
            cam.pos -= vec3{0, cam_speed, 0};
        break;
        case 'o':
            ct_var -= 1.0f;
            ct_var = ct_var < 0.0f ? 0.0f : ct_var;
        break;
        case 'p':
            ct_var += 1.0f;
            ct_var = ct_var > 90.0f ? 90.0f : ct_var;
        break;
        case 'l':
            control_index = (control_index + 1) % 3;
        break;
    }
}

void mouse(int button, int state, int x, int y)
{
    if (button == 3) // wheel up
    {
        
    }
    else if (button == 4) // wheel down
    {
        
    }
}

void light_enable()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float light_ambient[] = {0.2f, 0.2f, 0.2f};
    float light_specular[] = {1.0f, 0.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}

#ifndef TEST

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    int width = 700, height = 700;
    glutInitWindowSize(width, width);
    glutCreateWindow("CG Work");

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, (GLfloat)width / (GLfloat)height, 0.001f, 1000.0f);

    light_enable();

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    
    glutPassiveMotionFunc(motion);
    glutTimerFunc(0, timer, 0);

    // obj_file square("objs/object export.obj");
    // models[0].open("objs/object export.obj");
    // models.emplace_back(obj_file{"objs/object export.obj"});
    // models.emplace_back(obj_file{"objs/quad_square.obj"});
    // models.emplace_back(obj_file{"objs/cuboid.obj"});
    // models.emplace_back(obj_file{"objs/triangle.obj"});

    models.emplace_back(obj_file{"objs/quarto.obj"});

    glutMainLoop();
    return 0;
}

#endif
