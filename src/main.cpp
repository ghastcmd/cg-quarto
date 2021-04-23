#include "pch.h"

#include "vec.h"

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
    float yaw, pitch;
};

camera cam{ {0.0f, 0.0f, 5.0f}, {0.0f, 0.0f, -3.0f}, {0.0f, 1.0f, 0.0f}, -90.0f, 0 };

#define dist(vec) vec.x, vec.y, vec.z

float last_frame;
float fov = 75.0f;
float speed = 4.8f;
float mouse_sensitivity = 0.1f;

void timer(int count)
{    
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);
}

void display()
{
    last_frame = glutGet(GLUT_ELAPSED_TIME);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    vec3 look = cam.pos + cam.front;
    gluLookAt(dist(cam.pos), dist(look), dist(cam.up));

    glPushMatrix();
    glScalef(2.0f, 1.0f, 1.0f);
    glColor3f(0, 1, 0);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);
    glColor3f(1, 0, 0);
    glutSolidSphere(0.5, 20, 10);
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

    cam.pitch = std::clamp(cam.pitch, -89.0f, 89.0f);

    vec3 direction {
        cos(radians(cam.yaw)) * cos(radians(cam.pitch)),
        sin(radians(cam.pitch)),
        sin(radians(cam.yaw)) * cos(radians(cam.pitch))
    };

    cam.front = vec3::normalize(direction);
}

void keyboard(unsigned char key, int x, int y)
{
    float current_frame = glutGet(GLUT_ELAPSED_TIME);
    float delta_time = current_frame - last_frame;
    last_frame = current_frame;
    float cam_speed = speed * delta_time / 1000;
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

    glutMainLoop();
}