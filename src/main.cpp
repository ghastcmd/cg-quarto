#include "pch.h"
#include "window.hpp"
#include "vec.hpp"
#include "reader.hpp"

template <typename _ty>
constexpr _ty PI = (_ty)3.14159265358979323846;

float radians(float val)
{
    return val * (PI<float> / 180.0f);
}

static camera cam {
    {0.0f, 0.0f, 5.0f},
    {0.0f, 0.0f, -3.0f},
    {0.0f, 1.0f, 0.0f},
    {-90.0f, 0.0f, 0.0f}
};

float last_frame, dt;
float speed = 4.8f * 2.0f;
float mouse_sensitivity = 0.22f;

float fov = 75.0f;
float rot_angle = 0.0f, rot_speed = 0.4f;
float dw_angle_n_pos[] = {0.0f, 0.0f, 0.0f};
unsigned int control_index = 0;

void timer(int count)
{
    float current_frame = glutGet(GLUT_ELAPSED_TIME);
    dt = current_frame - last_frame;
    last_frame = current_frame;

    rot_angle = (rot_angle + rot_speed * dt);
    rot_angle -= 360.0f * (rot_angle >= 360.0f);

    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);
}

// std::vector<obj_file> models;
static std::unordered_map<const char*, obj_file> models;
obj_file simple;

static void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    cam.look_at();

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
            models["quarto"].draw_mesh();
        glPopMatrix();
    glPopMatrix();


    glPushMatrix(); // cama
        glScalef(2.0f, 2.0f, 2.0f);
        glTranslatef(5.0f, 0.0f, 2.0f);
        glTranslatef(-7.1f, -0.5f, 0.8f);
        glColor3f(0, 0, 1);
        models["cama"].draw_mesh();
    glPopMatrix();

    glPushMatrix(); // notebook
        glScalef(2.0f, 2.0f, 2.0f);
        glTranslatef(5.0f, 0.0f, 2.0f);
        glTranslatef(-6.8f, -0.22f, -0.2f);
        glColor3f(0, 0, 1);
        models["notebook"].draw_mesh();
    glPopMatrix();

    glPushMatrix(); //guarda-roupa
        glScalef(2.0f, 2.0f, 2.0f);
        glTranslatef(5.0f, 0.0f, 2.0f);
        glTranslatef(-3.5f, -0.1f, -0.2f);
        glColor3f(0, 0, 1);
        models["guardaroupa"].draw_mesh();
    glPopMatrix();

    glPushMatrix(); //mesa
        glScalef(2.0f, 2.0f, 2.0f);
        glTranslatef(5.0f, 0.0f, 2.0f);
        glTranslatef(-3.4f, -1.0f, 1.1f);
        glColor3f(0, 0, 1);
        models["mesa"].draw_mesh();
    glPopMatrix();


    glPushMatrix(); // caneca
        glScalef(2.0f, 2.0f, 2.0f);
        glTranslatef(5.0f, 0.0f, 2.0f);
        glTranslatef(-6.9f, -0.16f, -0.5f);
        glColor3f(0, 0, 1);
        models["caneca"].draw_mesh();
    glPopMatrix();

    glPushMatrix(); // cubo
        glScalef(2.0f, 2.0f, 2.0f);
        glTranslatef(5.0f, 0.0f, 2.0f);
        glTranslatef(-3.6f, -0.13f, 1.1f);
        glColor3f(0, 0, 1);
        models["cubo"].draw_mesh();
    glPopMatrix();

    glPushMatrix(); // caderno
        glScalef(2.0f, 2.0f, 2.0f);
        glTranslatef(5.0f, -0.19f, 1.2f);
        glTranslatef(-6.9f, 0.0f, 0.0f);
        glColor3f(0, 0, 1);
        models["caderno"].draw_mesh();
    glPopMatrix();

    glPushMatrix(); // cadeira
        glScalef(2.0f, 2.0f, 2.0f);
        glTranslatef(-1.3f, -0.71f, 1.5f);
        glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
        glColor3f(0, 0, 1);
        models["cadeira"].draw_mesh();
    glPopMatrix();

    glPushMatrix(); // ventilador
        glTranslatef(-3.6f, -0.39f, 1.5f);
        glRotatef(200.0f, 0.0f, 1.0f, 0.0f);
        glScalef(3.3f, 3.3f, 3.3f);
        glColor3f(0, 0, 1);
        models["ventiladorc"].draw_mesh();
        glTranslatef(0.0f, 0.175f, -0.11f);
        glRotatef(-rot_angle, 0.0f, 0.0f, 1.0f);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        models["ventiladorh"].draw_mesh();
    glPopMatrix();
    
    glutSwapBuffers();
}

window mwindow;

void reshape(int width, int height)
{
    mwindow.set_dimensions_values(width, height);
    mwindow.run_perspective();
}

void motion(int x, int y)
{
    static float prevx = mwindow.m_width / 2, prevy = mwindow.m_height / 2;
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
        case 'r':
            printf("%f %f %f\n", cam.front.x, cam.front.y, cam.front.z);
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

#ifndef TEST

int main(int argc, char **argv)
{
    mwindow.init(argc, argv, "CG Work", 700, 700);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float light_ambient[] = {0.2f, 0.2f, 0.2f};
    float light_diffuse[] = {0.2f, 0.2f, 0.2f};
    float light_specular[] = {1.0f, 0.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);  

    glEnable(GL_DEPTH_TEST);

    mwindow.set_display_func(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    
    glutPassiveMotionFunc(motion);
    glutTimerFunc(0, timer, 0);

    models["quarto"]      = obj_file("objs/quarto.obj");
    models["cama"]        = obj_file("objs/cama.obj");
    models["notebook"]    = obj_file("objs/notebook.obj");
    models["guardaroupa"] = obj_file("objs/guardaroupa.obj");
    models["mesa"]        = obj_file("objs/mesa.obj");
    models["caneca"]      = obj_file("objs/caneca.obj");
    models["cubo"]        = obj_file("objs/cubo.obj");
    models["caderno"]     = obj_file("objs/caderno.obj");
    models["cadeira"]     = obj_file("objs/cadeira.obj");
    models["ventiladorc"]  = obj_file("objs/ventilador_corpo.obj");
    models["ventiladorh"]  = obj_file("objs/ventilador_helice.obj");
    // models["ventilador2"] = obj_file("objs/ventilador2.obj");

    // models.emplace_back(obj_file{"objs/quarto.obj"});
    // models.emplace_back(obj_file{"objs/cama.obj"});
    // models.emplace_back(obj_file{"objs/notebook.obj"});
    // models.emplace_back(obj_file{"objs/guardaroupa.obj"});
    // models.emplace_back(obj_file{"objs/mesa.obj"});
    // models.emplace_back(obj_file{"objs/caneca.obj"});
    // models.emplace_back(obj_file{"objs/cubo.obj"});
    // models.emplace_back(obj_file{"objs/caderno.obj"});
    // models.emplace_back(obj_file{"objs/cadeira.obj"});
    // models.emplace_back(obj_file{"objs/ventilador.obj"});
    // models.emplace_back(obj_file{"objs/ventilador2.obj"});

    // main_window.create_window();
    mwindow.run();
    return 0;
}

#endif
