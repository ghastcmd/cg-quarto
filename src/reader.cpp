#include "pch.h"
#include "vec.h"
#include "reader.h"

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
    while (!isspace(*str)) str++;
    vec.y = atof(str++);
    while (!isspace(*str)) str++;
    vec.z = atof(str);
}

void stovec2(vec2& vec, char * str)
{
    vec.x = atof(str);
    while(!isspace(*str)) str++;
    vec.y = atof(str);
}

// struct obj_file
// {
obj_file::obj_file(const char *path)
{
    if (m_initialized) return;
    open(path);
    m_initialized = true;
}

char * take_tuple(unsigned int &v, unsigned int &t, unsigned int &n, char *str)
{
    v = atoi(str);
    while (*str != '/') str++;
    t = atoi(++str);
    while (*str != '/') str++;
    n = atoi(++str);
    while (*str != ' ' && *str) str++;

    return str;
}

void obj_file::open(const char *path)
{
    if (m_initialized) return;
    std::ifstream file(path, std::ios::in | std::ios::binary);
    while (file.peek() != -1)
    {
        const auto ret = translate(file);
        constexpr size_t max_strl = 64;
        // print_ret(ret);
        char str[max_strl];
        file.getline(str, max_strl, '\x0a');
        switch (ret)
        {
            case comment:
            case mtllib:
            case object_name:
            case smooth_shading:
            case usemtl:
                file.ignore(max_strl, '\x0a');
                break;
            break;
            case vertex_coord: {
                vec3 vec;
                stovec3(vec, str);
                vcoords.push_back(vec);
            } break;
            case vertex_texture: {
                vec2 vec;
                stovec2(vec, str);
                vtexture.push_back(vec);
            } break;
            case vertex_normal: {
                vec3 vec;
                stovec3(vec, str);
                vnormal.push_back(vec);
            } break;
            case face: {
                unsigned int vindexes[20] = {0};
                unsigned int tindexes[20] = {0};
                unsigned int nindexes[20] = {0};
                int i = 0, j = 0;
                char *stri = str;
                for (i = 0; *stri != '\0'; ++i)
                {
                    stri = take_tuple(vindexes[i], tindexes[i], nindexes[i], stri);
                }
                int len = i;
                // for (; vindexes[len] != 0; len++);
                unsigned int fvertex [128] = {0};
                unsigned int ftexture[128] = {0};
                unsigned int fnormals[128] = {0};
                fvertex[0] = vindexes[0];
                fvertex[1] = vindexes[1];
                fvertex[2] = vindexes[2];

                ftexture[0] = tindexes[0];
                ftexture[1] = tindexes[1];
                ftexture[2] = tindexes[2];

                fnormals[0] = nindexes[0];
                fnormals[1] = nindexes[1];
                fnormals[2] = nindexes[2];
                for (i = 0, j = 3; i + 3 <= len; ++i, j+=3)
                {
                    fvertex[j]   = vindexes[i];
                    fvertex[j+1] = vindexes[i+2];
                    fvertex[j+2] = vindexes[i+3];

                    ftexture[j]   = tindexes[i];
                    ftexture[j+1] = tindexes[i+2];
                    ftexture[j+2] = tindexes[i+3];

                    fnormals[j]   = nindexes[i];
                    fnormals[j+1] = nindexes[i+2];
                    fnormals[j+2] = nindexes[i+3];
                }
                len = j - 3;
                for (int i = 0; i < len; ++i)
                {
                    indices.push_back({fvertex[i]-1, ftexture[i]-1, fnormals[i]-1});
                }
            } break;
        }
    }
    file.close();
    m_initialized = true;
}

    // void data_buffer(unsigned int idx)
    // {
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
    // }

void obj_file::draw_mesh()
{
    //glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBegin(GL_TRIANGLES);
    // for (auto id: indices)
    for (int i = 0, len = indices.size(); i < len; ++i)
    {
        auto &id = indices[i];
        vec3 &vec = vcoords[id.vertex];
        glVertex3f(vec.x, vec.y, vec.z);
        vec2 &v2 = vtexture[id.texture];
        glTexCoord2f(v2.x, v2.y);
        vec = vnormal[id.normal];
        glNormal3f(vec.x, vec.y, vec.z);
    }
    glEnd();
}

    //unsigned int m_idx;
// };

#ifdef READER_TEST

int main()
{
    obj_file file("objs/cuboid.obj");

    // printf("%i\n", cnt);

    for (auto val: file.vcoords)
    {
        printf("%f %f %f\n", val.x, val.y, val.z);
    }

    for (auto val: file.indices)
    {
        printf("%i %i %i\n", val.vertex, val.texture, val.normal);
    }

    unsigned int test[] = {13, 11, 7, 13, 7, 9, 4, 3, 9, 4, 9, 7, 3, 7, 8, 8, 7, 11, 8, 11, 5, 7, 5, 6, 6, 2, 4, 6, 4, 8, 2, 1, 10, 2, 10, 3, 1, 3, 4, 6, 5, 12, 6, 12, 1, 5, 1, 2, 10, 13, 9, 10, 9, 3, 1, 12, 13, 1, 13, 10, 12, 5, 11, 12, 11, 13};

    for (int i = 0, max = file.indices.size(); i < max; ++i)
    {
        auto &vec = file.indices[i];
        if (vec.vertex != test[i]-1) puts("inconsistent value");
        printf("%i ", vec.vertex + 1);
    }
    puts("");

    for (auto &vec: file.vcoords)
    {
        printf("%f %f %f\n", vec.x, vec.y, vec.z);
    }

    float vertexes[] = {1, 1, -1, 1, -1, -1, 1, 1, 1, 1, -1, 1, -1, 1, -1, -1, -1, -1, -1, 1, 1, -1, -1, 1, 0, 1, 1, 1, 1, 0, -1, 1, 0, 0, 1, -1, 0, 2.575958, 0};

    for (int i = 0, j = 0, max = file.vcoords.size(); i < max; i++, j+=3)
    {
        auto &vec = file.vcoords[i];

        if (vertexes[j] != vec.x || vertexes[j + 1] != vec.y || vertexes[j + 2] != vec.z)
        {
            puts("inconsistent value");
        }
    }

    float textures[] = {0.75, 0.625, 0.875, 0.625, 0.875, 0.75, 0.75, 0.75, 0.375, 0.75, 0.625, 0.75, 0.625, 0.875, 0.625, 1.0, 0.375, 1.0, 0.375, 0.0, 0.625, 0.625, 0.125, 0.625, 0.25, 0.375, 0.25, 0.125, 0.5, 0.375, 0.5, 0.125, 0.75, 0.625, 0.5, 0.625, 0.625, 0.625, 0.375, 0.75, 0.5, 0.875, 0.5};
    for (int i = 0, j = 0, max = file.vtexture.size(); i < max; i++, j+=2)
    {
        auto &vec = file.vtexture[i];
        if (textures[j] != vec.x || textures[j+1] != vec.y) puts("inconsistent value");
        printf("%f %f | %f %f\n", textures[j], textures[j+1], vec.x, vec.y);
    }

    return 0;
}

#endif