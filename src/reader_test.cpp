#include "pch.h"
#include "vec.hpp"
#include "window.hpp"
#include "reader.hpp"

#ifdef READER_TEST

static unsigned int alloc_count = 0;

void * malloc_i(size_t size)
{
    alloc_count += 1;
    return malloc(size);
}

void * calloc_i(size_t count, size_t size)
{
    alloc_count += 1;
    return calloc(count, size);
}

void * realloc_i(void *ptr, size_t size)
{
    alloc_count += 1;
    return realloc(ptr, size);
}

void * operator new(size_t size)
{
    alloc_count += 1;
    return malloc(size);
}

// void * operator new(size_t size, void *ptr)
// {
//     alloc_count += 1;
//     return realloc(ptr, size);
// }

#define malloc(size) malloc_i(size)
#define calloc(count, size) calloc_i(count, size)
#define realloc(ptr, size) realloc(ptr, size)

#endif

#ifdef READER_TEST

static camera cam {
    {0.0f, 0.0f, 5.0f},
    {0.0f, 0.0f, -3.0f},
    {0.0f, 1.0f, 0.0f}
};

static std::vector<obj_file> models;
window windows;

static float dt, last_frame;

static void display(void)
{
    float current_frame = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    dt = current_frame - last_frame;
    last_frame = current_frame;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    cam.look_at();

    // glColor3f(0.2f, 0.2f, 0.2f);
    // glutSolidCube(1.0f);

    glPushMatrix();
        // glTranslatef(0.0f, 1.0f, 0.0f);
        glColor3f(1.0f, 0.0f, 0.0f);
        // glScalef(4.0f, 4.0f, 4.0f);
        models[0].draw_mesh();
    glPopMatrix();
    

    glutSwapBuffers();
}

static void reshape(int width, int height)
{
    windows.set_dimensions_values(width, height);
    windows.run_perspective();
}

static void motion(int x, int y)
{
    cam.motion(x, y, 0.22f);
}

static void keyboard(unsigned char key, int x, int y)
{
    float cam_speed = 8.55f * dt;
    switch(key)
    {
        case 'w':
            cam.pos += cam.front * cam_speed;
        break;
        case 's':
            cam.pos -= cam.front * cam_speed;
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
    }
}

static void timer(int count)
{
    glutPostRedisplay();
    glutTimerFunc(0, timer, 0);
}

int main(int argc, char **argv)
{
    windows.init(argc, argv, "Test Window", 900, 700);
    windows.set_display_func(display);
    cam.center_camera_angle(windows);

    glutReshapeFunc(reshape);
    glutPassiveMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);
    glEnable(GL_DEPTH_TEST);

    models.push_back(obj_file{"objs/cuboid.obj"});
    obj_file &file = models[0];

    unsigned int error_count = 0;

    for (auto val: file.vcoords)
    {
        printf("%9f %9f %9f\n", val.x, val.y, val.z);
    }

    for (auto val: file.indices)
    {
        printf("%2i %2i %2i\n", val.vertex, val.texture, val.normal);
    }

    unsigned int test[] = {13, 11, 7, 13, 7, 9, 4, 3, 9, 4, 9, 7, 3, 7, 8, 8, 7, 11, 8, 11, 5, 7, 5, 6, 6, 2, 4, 6, 4, 8, 2, 1, 10, 2, 10, 3, 1, 3, 4, 6, 5, 12, 6, 12, 1, 5, 1, 2, 10, 13, 9, 10, 9, 3, 1, 12, 13, 1, 13, 10, 12, 5, 11, 12, 11, 13};
    for (int i = 0, max = file.indices.size(); i < max; ++i)
    {
        auto &vec = file.indices[i];
        if (vec.vertex != test[i]-1)
        {
            puts("inconsistent value"), error_count += 1;
            printf("%2i ", vec.vertex + 1);
        }
    }
    puts("");

    // for (auto &vec: file.vcoords)
    // {
    //     printf("%9f %9f %9f\n", vec.x, vec.y, vec.z);
    // }

    float vertexes[] = {1, 1, -1, 1, -1, -1, 1, 1, 1, 1, -1, 1, -1, 1, -1, -1, -1, -1, -1, 1, 1, -1, -1, 1, 0, 1, 1, 1, 1, 0, -1, 1, 0, 0, 1, -1, 0, 2.575958, 0};

    for (int i = 0, j = 0, max = file.vcoords.size(); i < max; i++, j+=3)
    {
        auto &vec = file.vcoords[i];

        if (vertexes[j] != vec.x || vertexes[j + 1] != vec.y || vertexes[j + 2] != vec.z)
        {
            puts("inconsistent value");
            error_count += 1;
            printf("%9f %9f %9f\n", vertexes[j], vertexes[j+1], vertexes[j+2]);
        }
    }

    float textures[] = {0.75, 0.625, 0.875, 0.625, 0.875, 0.75, 0.75, 0.75, 0.375, 0.75, 0.625, 0.75, 0.625, 0.875, 0.625, 1.0, 0.375, 1.0, 0.375, 0.0, 0.625, 0.0, 0.625, 0.125, 0.625, 0.25, 0.375, 0.25, 0.125, 0.5, 0.375, 0.5, 0.125, 0.75, 0.625, 0.5, 0.625, 0.625, 0.625, 0.375, 0.75, 0.5, 0.875, 0.5};
    for (int i = 0, j = 0, max = file.vtexture.size(); i < max; i++, j+=2)
    {
        auto &vec = file.vtexture[i];
        if (textures[j] != vec.x || textures[j+1] != vec.y)
        {
            puts("inconsistent value");
            error_count += 1;
            printf("%f %f | %f %f\n", textures[j], textures[j+1], vec.x, vec.y);
        }
    }

    float normals[] = {-0.5263, 0.6679, 0.5263, 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.5263, 0.6679, 0.5263, 0.5263, 0.6679, -0.5263, -0.5263, 0.6679, -0.5263};
    for (int i = 0, j = 0, max = file.vnormal.size(); i < max; ++i, j+=3)
    {
        auto &vec = file.vnormal[i];
        if (normals[j] != vec.x || normals[j+1] != vec.y || normals[j+2] != vec.z)
        {
            puts("inconsistent value");
            error_count += 1;
            printf("%9f %9f %9f | %9f %9f %9f\n", normals[j], normals[j+1], normals[j+2], vec.x, vec.y, vec.z);
        }
    }

    const char *fmt_status_error[] {
        "Error count >> %i\n",
        "Tets runned without an error\n"
    };
    printf(fmt_status_error[error_count == 0], error_count);

    const char *fmt_alloc_count[] {
        "Allocation count >> %i\n",
        "No allocations were executed in the runnig of the program\n"
    };
    printf(fmt_alloc_count[alloc_count == 0], alloc_count);

    windows.run();
    return 0;
}

#endif