#include "pch.h"
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

#include <string>
#include <string.h>
#include <string_view>

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

int main()
{
    std::ifstream file("objs/object export.obj", std::ios::in | std::ios::binary);

    //while (!file.eof())
    
    //for (int i = 0; i < 15; i++)
    while (file.peek() != -1)
    {
        auto ret = translate(file);
        print_ret(ret);
        char str[32];
        file.getline(str, 32, '\x0a');
        puts(str);
    }

    //std::string_view simple_str;
    //std::string_view simple_str(str, str);
    //std::string new_str(str);

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
