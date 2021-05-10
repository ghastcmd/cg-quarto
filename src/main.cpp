#include "pch.h"
#include "window.hpp"
#include "vec.hpp"
#include "reader.hpp"
#include "light.hpp"

static camera cam {
    {0.0f, 0.0f, 5.0f},
    {0.0f, 0.0f, -3.0f},
    {0.0f, 1.0f, 0.0f},
};

float last_frame = 0, dt = 0;
float speed = 9.6f;

float rot_angle = 0.0f;
float dw_angle_n_pos[] = {0.0f, 0.0f, 0.0f};
unsigned int control_index = 0;

static void idle()
{
    float current_frame = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    dt = current_frame - last_frame;
    last_frame = current_frame;

    float rotation_speed = 360.0f * 2.0f;
    rot_angle += rotation_speed * dt;
    rot_angle -= 360.0f * (rot_angle >= 360.0f);

    glutPostRedisplay();
}

static std::unordered_map<const char*, obj_file> models;

material mat1 {{1.0f, 1.0f, 1.0f}, {0.8f, 0.8f, 0.8f}, {0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, 255.0f};

clight light[2];

static void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    cam.look_at();

    for (auto &li: light)
    {
        li.dist_position();
    }

    mat1.apply_material();

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
            glTranslatef(-5.0f, -1.0f, 0.0f);
            models["quarto"].draw_mat_mesh();
        glPopMatrix();
    glPopMatrix();


    glPushMatrix(); // cama
        glScalef(2.0f, 2.0f, 2.0f);
        glTranslatef(-2.1f, -0.48f, 2.8f);
        models["cama"].draw_mat_mesh();
    glPopMatrix();

    glPushMatrix(); // notebook
        glScalef(2.0f, 2.0f, 2.0f);
        glTranslatef(-1.8f, -0.22f, 1.8f);
        models["notebook"].draw_mat_mesh();
    glPopMatrix();

    glPushMatrix(); //guarda-roupa
        glTranslatef(0.8f, -1.9f, 3.5f);
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        glScalef(2.0f, 2.0f, 2.0f);
        models["guardaroupa"].draw_mat_mesh();
    glPopMatrix();

    glPushMatrix(); //mesa
        glScalef(2.0f, 2.0f, 2.0f);
        glTranslatef(5.0f, 0.0f, 2.0f);
        glTranslatef(-3.4f, -1.0f, 1.1f);
        models["mesa"].draw_mat_mesh();
    glPopMatrix();


    glPushMatrix(); // caneca
        glScalef(2.0f, 2.0f, 2.0f);
        glTranslatef(-1.9f, -0.15f, 1.5f);
        glScalef(0.05f, 0.05f, 0.05f);
        models["caneca"].draw_mat_mesh();
    glPopMatrix();

    glPushMatrix(); // cubo
        glTranslatef(2.8f, -0.51f, 6.7f);
        glScalef(2.0f, 2.0f, 2.0f);
        models["cubo"].draw_mat_mesh();
    glPopMatrix();

    glPushMatrix(); // caderno
        glTranslatef(2.9f, -0.56f, 6.2f);
        glRotatef(80.0f, 0.0f, 1.0f, 0.0f);
        glScalef(2.0f, 2.0f, 2.0f);
        models["caderno"].draw_mat_mesh();
    glPopMatrix();

    glPushMatrix(); // cadeira
        glScalef(2.0f, 2.0f, 2.0f);
        glTranslatef(-1.3f, -0.71f, 1.5f);
        glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
        models["cadeira"].draw_mat_mesh();
    glPopMatrix();

    glPushMatrix(); // ventilador
        glTranslatef(-3.6f, -0.39f, 1.5f);
        glRotatef(200.0f, 0.0f, 1.0f, 0.0f);
        glScalef(3.3f, 3.3f, 3.3f);
        models["ventiladorc"].draw_mat_mesh();
        glTranslatef(0.0f, 0.175f, -0.11f);
        glRotatef(rot_angle, 0.0f, 0.0f, 1.0f);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        models["ventiladorh"].draw_mat_mesh();
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(3.5f, -3.08f, 5.8f);
        glRotatef(-220.0f, 0.0f, 1.0f, 0.0f);
        glScalef(2.2f, 2.2f, 2.2f);
        models["luminaria"].draw_mat_mesh();
    glPopMatrix();

    glPushMatrix();
        models["quadro van"].draw_mat_mesh();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.0f);
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        models["janela"].draw_mat_mesh();
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
    cam.motion(x, y, 0.22f);
}

bool light_on = true;

void keyboard(unsigned char key, int x, int y)
{
    float cam_speed = speed * dt;
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
            cam.pos -= cam.side_vector() * cam_speed;
        break;
        case 'd':
            cam.pos += cam.side_vector() * cam_speed;
        break;
        case ' ':
            cam.pos += cam.up * cam_speed;
        break;
        case 'b':
            cam.pos -= cam.up * cam_speed;
        break;
        case 'o':
            ct_var -= 1.0f;
            ct_var = (ct_var >= 0.0f) * ct_var;
        break;
        case 'p':
            ct_var += 1.0f;
            ct_var = ct_var > 90.0f ? 90.0f : ct_var;
        break;
        case 'l':
            control_index = (control_index + 1) % 3;
        break;
        case 'r':
            static GLenum mode = GL_LINE;
            glPolygonMode(GL_FRONT_AND_BACK, mode);
            mode = mode == GL_LINE ? GL_FILL : GL_LINE;
        break;
        case 'k':
            if (light_on)
            {
                light[1].disable();
                light_on = false;
            }
            else
            {
                light[1].enable();
                light_on = true;
            }
        break;
        case 'u':
            printf("%f %f %f\n", cam.pos.x, cam.pos.y, cam.pos.z);
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

    glEnable(GL_DEPTH_TEST);
    /* lightning  */
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);
    
    light[0] = clight(
        0,
        {0.488164f, 3.054233f, 6.129874f, 0.0f}, // position
        {0.0f, 0.0f, 0.0f, 0.0f}, // direction
        {0.0f, 0.0f, 0.0f, 0.0f}, // ambient
        {1.0f, 1.0f, 1.0f, 1.0f}, // diffuse
        {1.0f, 1.0f, 1.0f, 1.0f}, // specular
        0.0f, 0.0f, clight::type::point_light
    );

    light[1] = clight(
        1,
        {3.376924f, -0.035178f, 5.941844f, 1.0f}, // position
        {2.642475f, -0.578486f, 6.341764f, 1.0f},   // direction
        {0.2f, 0.2f, 0.2f, 1.0f},   // ambient
        {2.0f, 2.2f, 2.0f, 2.0f},   // diffuse
        {1.0f, 1.0f, 1.0f, 1.0f},   // specular
        5.0f, 100.0f, clight::type::spot_light
    );

    for (auto &li: light)
    {
        li.enable();
        li.apply_color();
    }

    mwindow.set_display_func(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);
    glutPassiveMotionFunc(motion);

    cam.center_camera_angle(mwindow);

    models["quarto"]      = obj_file("objs/quarto.obj");
    models["cama"]        = obj_file("objs/cama.obj");
    models["notebook"]    = obj_file("objs/notebook.obj");
    models["guardaroupa"] = obj_file("objs/guardaroupa.obj");
    models["mesa"]        = obj_file("objs/mesa.obj");
    models["caneca"]      = obj_file("objs/caneca.obj");
    models["cubo"]        = obj_file("objs/cubomagico.obj");
    models["caderno"]     = obj_file("objs/caderno.obj");
    models["cadeira"]     = obj_file("objs/cadeira.obj");
    models["ventiladorc"] = obj_file("objs/ventilador_corpo.obj");
    models["ventiladorh"] = obj_file("objs/ventilador_helice.obj");
    models["luminaria"]   = obj_file("objs/luminaria.obj");
    models["quadro van"]  = obj_file("objs/quadroVangog.obj");
    models["janela"]      = obj_file("objs/janela.obj");

    mwindow.run();
    return 0;
}

#endif
