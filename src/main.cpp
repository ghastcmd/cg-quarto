#include "pch.h"

#include "vec.h"

void timer(int count)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);
}

vec3 cam{0.0f, 0.0f, 5.0f};
vec3 cam_front{0.0f, 0.0f, -3.0f};

#define dist(vec) vec.x, vec.y, vec.z

float last_frame;
float fov = 75.0f;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    vec3 look = cam + cam_front;
    gluLookAt(dist(cam), dist(look), 0.0f, 1.0f, 0.0f);

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
    static int bef_x = 0, bef_y = 0;
    float dir_x = (x > bef_x) - (x < bef_x);
    float dir_y = (y < bef_y) - (y > bef_y);
    dir_x /= 10;
    dir_y /= 10;

    cam_front.x = (cam_front.x + dir_x);
    cam_front.x -= 360 * (cam_front.x > 360); 
    cam_front.y = (cam_front.y + dir_y);
    cam_front.y -= 360 * (cam_front.y > 360);

    printf("%i %i %f %f %f %f\n", x, y, dir_x, dir_y, cam_front.x, cam_front.y);
    bef_x = x, bef_y = y;
}

void keyboard(unsigned char key, int x, int y)
{
    float current_frame = glutGet(GLUT_ELAPSED_TIME);
    float delta_time = current_frame - last_frame;
    last_frame = current_frame;
    float cam_speed = 0.5f * delta_time / 1000;
    switch (key)
    {
        case 'w':
            cam += cam_front * cam_speed;
        break;
        case 's':
            cam -= cam_front * cam_speed;
        break;
        case 'a':
            cam -= vec3::normalize(vec3::cross(cam_front, {0,1,0})) * cam_speed;
        break;
        case 'd':
            cam += vec3::normalize(vec3::cross(cam_front, {0,1,0})) * cam_speed;
        break;
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    int width = 700, height = 700;
    glutInitWindowSize(width, width);
    glutCreateWindow("simple title");

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, (GLfloat)width / (GLfloat)height, 0.001f, 1000.0f);

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(motion);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
}

/**
void motion(int x, int y){
	//cam_rotation[X] += dt;
	static int last[2];
	cam_rotation[Y] -= (x - last[X]) * sensibility[X] * dt;
	cam_rotation[X] -= (y - last[Y]) * sensibility[Y] * dt;

	last[X] = x;
	last[Y] = y;
}
 */