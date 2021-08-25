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

// void * operator new(size_t size)
// {
//     alloc_count += 1;
//     return malloc(size);
// }

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    cam.look_at();

    glColor3f(0.2f, 0.2f, 0.2f);
    glutSolidCube(1.0f);

    glPushMatrix();
        glTranslatef(0.0f, 1.0f, 0.0f);
        glScalef(4.0f, 4.0f, 4.0f);
        models[0].draw_mat_mesh();
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
    const vec2 camera_cap {-30.0f, 30.0f};
    cam.motion_capped(x, y, 0.22f, vec2{-90.0f, -90.0f} + camera_cap, camera_cap);
    // cam.motion(x, y, 0.22f);
}

static void idle()
{
    float current_frame = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    dt = current_frame - last_frame;
    last_frame = current_frame;
    
    glutPostRedisplay();
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
        case 'r':
            printf("%f %f %f\n", cam.yaw, cam.pitch, cam.roll);
        break;
    }
}

extern void merge_path_name(const char*, char*, const size_t);
extern void merge_path_name(const char*, char*);

int main(int argc, char **argv)
{
    windows.init(argc, argv, "Test Window", 900, 700);
    windows.set_display_func(display);
    cam.center_camera_angle(windows);

    glutReshapeFunc(reshape);
    glutPassiveMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);

    float ambient_n_diffuse[] {0.2f, 0.2f, 0.2f};
    float specular[] {1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, ambient_n_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    models.push_back(obj_file{"objs/cuboid.obj"});
    obj_file &file = models[0];

    unsigned int error_count = 0;

    const char *path[] = 
    {
        "",
        "simple/file",
        "simple/fiile/alelo.jpg"
    };
    char name[][50]
    {
        "name.mtl",
        "another_name.mtl",
        "cupid.mtl"
    };

    const char *correct[]
    {
        "name.mtl",
        "simple/another_name.mtl",
        "simple/fiile/cupid.mtl"
    };

    for (int i = 0; i < 3; i++)
    {
        merge_path_name(path[i], name[i]);
        if (strcmp(name[i], correct[i]))
        {
            error_count += 1;
            printf("%s | %s   >> line %i\n", name[i], correct[i], __LINE__);
        }
    }

    unsigned int test[] = {13, 11, 7, 13, 7, 9, 4, 3, 9, 4, 9, 7, 3, 7, 8, 8, 7, 11, 8, 11, 5, 7, 5, 6, 6, 2, 4, 6, 4, 8, 2, 1, 10, 2, 10, 3, 1, 3, 4, 6, 5, 12, 6, 12, 1, 5, 1, 2, 10, 13, 9, 10, 9, 3, 1, 12, 13, 1, 13, 10, 12, 5, 11, 12, 11, 13};
    if (std::size(test) != file.indices.size())
    {
        printf("size lenghts don't match %i | %i | line %i\n", std::size(test), file.indices.size(), __LINE__);
        error_count += 1;
    }
    for (int i = 0, max = file.indices.size(); i < max; ++i)
    {
        auto &vec = file.indices[i];
        if (vec.vertex != test[i]-1)
        {
            printf("insconsistent value <%i> %i\n", i, __LINE__);
            printf("%u | %u\n", vec.vertex, test[i] - 1);
            error_count += 1;
        }
    }

    float vertexes[] = {1, 1, -1, 1, -1, -1, 1, 1, 1, 1, -1, 1, -1, 1, -1, -1, -1, -1, -1, 1, 1, -1, -1, 1, 0, 1, 1, 1, 1, 0, -1, 1, 0, 0, 1, -1, 0, 2.575958, 0};
    if (std::size(vertexes) != file.vcoords.size() * 3)
    {
        printf("size lenghts don't match %i\n", __LINE__);
        error_count += 1;
    }
    for (int i = 0, j = 0, max = file.vcoords.size(); i < max; i++, j+=3)
    {
        auto &vec = file.vcoords[i];

        if (vertexes[j] != vec.x || vertexes[j + 1] != vec.y || vertexes[j + 2] != vec.z)
        {
            printf("inconssitent value index<%i> line: %i\n", j, __LINE__);
            error_count += 1;
            printf("%9f %9f %9f | %9f %9f %9f\n", vertexes[j], vertexes[j+1], vertexes[j+2], vec.x, vec.y, vec.z);
        }
    }

    float textures[] = {0.75, 0.625, 0.875, 0.625, 0.875, 0.75, 0.75, 0.75, 0.375, 0.75, 0.625, 0.75, 0.625, 0.875, 0.625, 1.0, 0.375, 1.0, 0.375, 0.0, 0.625, 0.0, 0.625, 0.125, 0.625, 0.25, 0.375, 0.25, 0.125, 0.5, 0.375, 0.5, 0.125, 0.75, 0.625, 0.5, 0.625, 0.625, 0.625, 0.375, 0.75, 0.5, 0.875, 0.5};
    if (std::size(textures) != file.vtexture.size() * 2)
    {
        printf("size lenghts don't match %i\n", __LINE__);
        error_count += 1;
    }
    for (int i = 0, j = 0, max = file.vtexture.size(); i < max; i++, j+=2)
    {
        auto &vec = file.vtexture[i];
        if (textures[j] != vec.x || textures[j+1] != vec.y)
        {
            printf("inconssitent value %i\n", __LINE__);
            error_count += 1;
            printf("%f %f | %f %f\n", textures[j], textures[j+1], vec.x, vec.y);
        }
    }

    float normals[] = {-0.5263, 0.6679, 0.5263, 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.5263, 0.6679, 0.5263, 0.5263, 0.6679, -0.5263, -0.5263, 0.6679, -0.5263};
    if (std::size(normals) != file.vnormal.size() * 3)
    {
        printf("size lenghts don't match %i\n", __LINE__);
        error_count += 1;
    }
    for (int i = 0, j = 0, max = file.vnormal.size(); i < max; ++i, j+=3)
    {
        auto &vec = file.vnormal[i];
        if (normals[j] != vec.x || normals[j+1] != vec.y || normals[j+2] != vec.z)
        {
            printf("inconssitent value %i\n", __LINE__);
            error_count += 1;
            printf("%9f %9f %9f | %9f %9f %9f\n", normals[j], normals[j+1], normals[j+2], vec.x, vec.y, vec.z);
        }
    }

    // mtl_file mtl_file ("objs/cuboid.mtl");
    auto mtl_file = file.mat_lib;

    if (int len = mtl_file.mat_names.size(); len != 1)
    {
        error_count += 1;
        printf("The number of named materials doesn't match\n\tExpected 1, got %i, line: %i\n", len, __LINE__);
    }
    for (auto &val: mtl_file.mat_names)
    {
        if (strncmp(val.c_str(), "Material", std::size("Material") - 1))
        {
            error_count += 1;
            printf("%s\n", val.c_str());
        }
    }
    
    mtl_file.materials[0].apply_material();
    // mtl_file.materials[0].tex_diffuse.bind();

    if (int len = mtl_file.materials.size(); len != 1)
    {
        error_count += 1;
        printf("The number of materials doesn't match\n\tExpected 1, got %i, line: %i\n", len, __LINE__);
    }
    for (auto &val: mtl_file.materials)
    {
        if (val.ambient.x != 1.0f || val.ambient.y != 1.0f || val.ambient.z != 1.0f)
        {
            error_count += 1;
            printf("ambient: %f %f %f\n", val.ambient.x, val.ambient.y, val.ambient.z);
        }
        if (val.diffuse.x != 0.8f || val.diffuse.y != 0.8f || val.diffuse.z != 0.8f)
        {
            error_count += 1;
            printf("diffuse: %f %f %f\n", val.diffuse.x, val.diffuse.y, val.diffuse.z);
        }
        if (val.specular.x != 0.5f || val.specular.y != 0.5f || val.specular.z != 0.5f)
        {
            error_count += 1;
            printf("specular: %f %f %f\n", val.specular.x, val.specular.y, val.specular.z);
        }
        if (val.emissive.x != 0.0f || val.emissive.y != 0.0f || val.emissive.z != 0.0f)
        {
            error_count += 1;
            printf("emission: %f %f %f\n", val.emissive.x, val.emissive.y, val.emissive.z);
        }
        if (ceil(val.highlights) != 323.999994f)
        {
            error_count += 1;
            printf("highlights: %.20lf\n", val.highlights);
        }
        if (val.optical_density != 1.45f)
        {
            error_count += 1;
            printf("optical density: %f\n", val.optical_density);
        }
        if (val.dissolve != 1.0f)
        {
            error_count += 1;
            printf("shininess: %f\n", val.highlights);
        }
    }

    if (mtl_file.materials[0].tex_diffuse.m_init == false)
    {
        error_count += 1;
        puts("Have not initialized texture object");
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