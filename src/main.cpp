#include "pch.h"

void timer(int count)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);
}

float caml_x = 0, caml_y = 0, caml_z = 0;
float fov = 75.0f;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0.0f, 1.3f, 5.0f, caml_x, caml_y, caml_z, 0.0f, 1.0f, 0.0f);

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

    caml_x = (caml_x + dir_x);
    caml_x -= 360 * (caml_x > 360); 
    caml_y = (caml_y + dir_y);
    caml_y -= 360 * (caml_y > 360);

    printf("%i %i %f %f %f %f\n", x, y, dir_x, dir_y, caml_x, caml_y);
    bef_x = x, bef_y = y;
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