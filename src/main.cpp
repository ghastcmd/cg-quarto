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
float speed = 4.8f;
float mouse_sensitivity = 0.1f;

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

vec3 test = {-3.62f, 0.055f, 2.5f}, test_scale = {0.95f, 0.94f, 1.0f}, test_rotation = {0};
float test_angle = 0;
float d_angle = 0;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    vec3 look = cam.pos + cam.front;
    gluLookAt(dist(cam.pos), dist(look), dist(cam.up));

    glPushMatrix();
        glTranslatef(test.x, test.y, test.z);
        glRotatef(test_angle, test_rotation.x, test_rotation.y, test_rotation.z);
        glScalef(test_scale.x, test_scale.y, test_scale.z);
        float door_width = 0.8f;
        glRotatef(d_angle, 0, 1, 0);
        glTranslatef(door_width / 2, 0, 0);
        glScalef(door_width, 2.1f, 0.03f);
        glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -1.0f, 3.0f);
    glColor3f(0, 0, 1);
    models[0].draw_mesh();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2.0f, -1.0f, 3.0f);
    glColor3f(0, 0, 1);
    models[1].draw_mesh();
    glPopMatrix();

    glPushMatrix();
    glColor3f(1, 0, 0);
    glTranslatef(3.0f, -1.0f, 0.0f);
    glScalef(0.5f, 0.5f, 0.5f);
    models[2].draw_mesh();
    glPopMatrix();

    glPushMatrix();
    glColor3f(1, 0, 0);
    glTranslatef(-5.0f, -1.0f, 0.0f);
    models[4].draw_mesh();
    glPopMatrix();

    glPushMatrix();
        glColor3f(0, 0, 1);
        glTranslatef(0.0f, 1.0f, 0.0f);
        glBegin(GL_TRIANGLES);
        
        glVertex3f(-1.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f);
        glNormal3f(0.0f, 1.0f, 0.0f);

        glVertex3f(1.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f);
        glNormal3f(0.0f, 1.0f, 0.0f);

        glVertex3f(-1.0f, 0.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f);
        glNormal3f(0.0f, 1.0f, 0.0f);

        glEnd();
    glPopMatrix();

    glColor3f(1.0f, 0.8f, 0.9f);
    simple.draw_mesh();

    glPushMatrix();
    glColor3f(0.3, 0.5f, 1.0f);
    glTranslatef(-10.0f, 0.0f, 0.0f);
    models[3].draw_mesh();
    glEnd();
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
        case 'r':
            printf("%f %f %f\n", test.x, test.y, test.z);
            printf("%f %f %f\n", test_scale.x, test_scale.y, test_scale.z);
            printf("%f %f %f %f\n", test_angle, test_rotation.x, test_rotation.y, test_rotation.z);
        break;
        case 'u':
            test.z += 0.1f;
        break;
        case 'j':
            test.z -= 0.1f;
        break;
        case 'k':
            test.x += 0.1f;
        break;
        case 'h':
            test.x -= 0.1f;
        break;
        case 'm':
            test.y += 0.1f;
        break;
        case 'n':
            test.y -= 0.1f;
        break;
    }
}

void mouse(int button, int state, int x, int y)
{
    if (button == 3) // wheel up
    {
        d_angle += 1.0f;
        d_angle = d_angle > 90.0f ? 90.0f : d_angle;
    }
    else if (button == 4) // wheel down
    {
        d_angle -= 1.0f;
        d_angle = d_angle < 0.0f ? 0.0f : d_angle;
    }
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

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    
    glutPassiveMotionFunc(motion);
    glutTimerFunc(0, timer, 0);

    // obj_file square("objs/object export.obj");
    // models[0].open("objs/object export.obj");
    models.emplace_back(obj_file{"objs/object export.obj"});
    models.emplace_back(obj_file{"objs/quad_square.obj"});
    models.emplace_back(obj_file{"objs/cuboid.obj"});
    models.emplace_back(obj_file{"objs/triangle.obj"});

    // puts("printing model");
    models.emplace_back(obj_file{"objs/quarto.obj"});

    simple.vcoords.push_back({-1, 0, 1});
    simple.vcoords.push_back({ 1, 0, 1});
    simple.vcoords.push_back({-1, 0,-1});

    simple.vtexture.push_back({0, 0});
    simple.vtexture.push_back({0, 0});
    simple.vtexture.push_back({0, 0});

    simple.vnormal.push_back({0, 1, 0});

    simple.indices.push_back({0, 0, 0});
    simple.indices.push_back({1, 1, 0});
    simple.indices.push_back({2, 2, 0});

    glutMainLoop();
    return 0;
}

#endif
