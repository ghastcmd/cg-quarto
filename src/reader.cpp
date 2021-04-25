#include "pch.h"
#include "vec.h"
#include <stdlib.h>

int cnt = 0;
void * i_malloc(size_t size)
{
    cnt += 1;
    return malloc(size);
}

void *operator new(size_t size)
{
    cnt += 1;
    return malloc(size);
}

#undef malloc
#define malloc(size) i_malloc(size)

enum type_ret
{
    invalid,
    comment,
    mtllib,
    object_name,
    vertex,
    vertex_coord,
    vertex_texture,
    vertex_normal,
    usemtl,
    smooth_shading,
    face
};

type_ret translate(std::ifstream& file)
{
    char str[15];
    file.getline(str, 15, ' ');
    if (!strcmp(str, "#"))
        return comment;
    else if (!strcmp(str, "mtllib"))
        return mtllib;
    else if (!strcmp(str, "o"))
        return object_name;
    else if (!strcmp(str, "vn"))
        return vertex_normal;
    else if (!strcmp(str, "vt"))
        return vertex_texture;
    else if (!strcmp(str, "v"))
        return vertex_coord;
    else if (!strcmp(str, "usemtl"))
        return usemtl;
    else if (!strcmp(str, "s"))
        return smooth_shading;
    else if (!strcmp(str, "f"))
        return face;

    return invalid;
}

void print_ret(type_ret ret)
{
    switch (ret)
    {
        case comment:
            puts("comment");
        break;
        case mtllib:
            puts("mtllib");
        break;
        case object_name:
            puts("object_name");
        break;
        case vertex_normal:
            puts("vertex_normal");
        break;
        case vertex_texture:
            puts("vertex_texture");
        break;
        case vertex_coord:
            puts("vertex_coord");
        break;
        case usemtl:
            puts("usemtl");
        break;
        case smooth_shading:
            puts("smooth_shading");
        break;
        case face:
            puts("face");
        break;
        case invalid:
            puts("invalid");
        break;
    }
}

void print_val(std::ifstream& fstream)
{
    char str[16];
    fstream.getline(str, 16, '\x0a');
    puts(str);
}

void stovec3(vec3& vec, char * str)
{
    vec.x = atof(str);
    while (*str != ' ') str++;
    vec.y = atof(str++);
    while (*str != ' ') str++;
    vec.z = atof(str);
}

void stovec2(vec2& vec, char * str)
{
    vec.x = atof(str);
    while(*str != ' ') str++;
    vec.y = atof(str);
}

struct obj_file
{
    obj_file(const char *path)
    {
        std::ifstream file(path, std::ios::in | std::ios::binary);
        while (file.peek() != -1)
        {
            auto ret = translate(file);
            print_ret(ret);
            char str[32];
            file.getline(str, 32, '\x0a');
            switch (ret)
            {
                case comment:
                case mtllib:
                case object_name:
                case smooth_shading:
                case usemtl:
                    file.ignore(32, '\x0a');
                    break;
                break;
                case vertex_coord:
                {
                    vec3 vec;
                    stovec3(vec, str);
                    vcoords.push_back(vec);
                }
                break;
                case vertex_texture:
                {
                    vec2 vec;
                    stovec2(vec, str);
                    vtexture.push_back(vec);
                }
                break;
                case vertex_normal:
                {
                    vec3 vec;
                    stovec3(vec, str);
                    vnormal.push_back(vec);
                }
                break;
                case face:
                {
                    unsigned int idxs[20] = {0};
                    idxs[0] = atoi(str);
                    int i, j;
                    char *stri = str;
                    for (i = 1; i < 32 && *stri; ++i)
                    {
                        while (*stri != ' ') stri++;
                        idxs[i] = atoi(stri++);
                    }
                    int len = 0;
                    for (int i = 0; idxs[i] != 0; ++i, ++len);
                    unsigned int final_idx[64];
                    final_idx[0] = idxs[0];
                    final_idx[1] = idxs[1];
                    final_idx[2] = idxs[2];
                    for (i = 0, j = 3; i + 3 < len; ++i, j+=3)
                    {
                        final_idx[j] = idxs[i];
                        final_idx[j+1] = idxs[i+2];
                        final_idx[j+2] = idxs[i+3];
                    }
                    for (int len = j, i = 0; i < len; ++i)
                    {
                        indices.push_back(final_idx[i]-1);
                    }
                }
                break;
            }
        }
        file.close();
    }

    void data_buffer(unsigned int idx)
    {
        /*m_idx = idx;
        unsigned int buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(
            GL_ARRAY_BUFFER,
            vcoords.size() * 3 * sizeof(float), 
            vcoords.data(), 
            GL_STATIC_DRAW
        );

        glEnableVertexAttribArray(idx);
        glVertexAttribPointer(idx, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

        unsigned int ibo;
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER, 
            indices.size() * sizeof(unsigned int), 
            indices, 
            GL_STATIC_DRAW
        );*/
    }

    void draw_mesh()
    {
        //glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
        for (int i = 0, len = indices.size(); i < len; ++i)
        {
            vec3 vec = vcoords[indices[i]];
            glVertex3f(vec.x, vec.y, vec.z);
        }
    }

    //unsigned int m_idx;

    std::vector<vec3> vcoords;
    std::vector<vec3> vnormal;
    std::vector<vec2> vtexture;

    std::vector<unsigned int> indices; 
};

int main()
{
    obj_file file("objs/object export.obj");

    printf("%i\n", cnt);

    for (auto val: file.indices)
    {
        printf("%i ", val);
    }
    puts("");

    return 0;
}

/**
 * unsigned int buffer;
 * glGenBuffers(1, &buffer);
 * glBindBuffer(GL_ARRAY_BUFFER, buffer);
 * glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);
 * 
 * glEnableVertexAttribArray(0);
 * glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
 * 
 * unsigned int ibo;
 * glGenBuffers(1, &ibo);
 * glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
 * glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
 * 
 * 
 * before...
 * glDrawArrays(GL_TRIANGLES, 0, 6);
 * 
 * when drawing
 * glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); // this draws the elements
 */
