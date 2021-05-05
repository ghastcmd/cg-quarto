#include "pch.h"
#include "vec.hpp"
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

enum objtypes
{
    invalid,
    comment,
    mtllib,
    object_name,
    vertex_coord,
    vertex_texture,
    vertex_normal,
    usemtl,
    smooth_shading,
    face,
    line
};

static std::unordered_map<std::string_view, objtypes> objtypes_map {
    {"#", comment},
    {"mtllib", mtllib},
    {"o", object_name},
    {"v", vertex_coord},
    {"vn", vertex_normal},
    {"vt", vertex_texture},
    {"usemtl", usemtl},
    {"f", face},
    {"l", line}
};

void print_ret(objtypes ret)
{
    const char *objtypes_str[] {
        "invalid",
        "comment",
        "mtllib",
        "object_name",
        "vertex_coord",
        "vertex_texture",
        "vertex_normal",
        "usemtl",
        "smooth_shading",
        "face",
        "line"
    };

    puts(objtypes_str[ret]);
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

void obj_file::get_faces_index(char *str)
{
    constexpr unsigned int buffer_size = 30;
    unsigned int vindexes[buffer_size] = {0};
    unsigned int tindexes[buffer_size] = {0};
    unsigned int nindexes[buffer_size] = {0};
    int i = 0, j = 0;
    for (i = 0; *str != '\0'; ++i)
    {
        str = take_tuple(vindexes[i], tindexes[i], nindexes[i], str);
    }
    int len = i;
    unsigned int fvertex [buffer_size * 3] = {0};
    unsigned int ftexture[buffer_size * 3] = {0};
    unsigned int fnormals[buffer_size * 3] = {0};
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
    { // Converting n edge faces to triangles
        fvertex[j]   = vindexes[i];
        fvertex[j+1] = vindexes[i+2];
        fvertex[j+2] = vindexes[i+3];

        ftexture[j]   = tindexes[i];
        ftexture[j+1] = tindexes[i+2];
        ftexture[j+2] = tindexes[i+3];

        fnormals[j]   = nindexes[i];
        fnormals[j+1] = nindexes[i+1];
        fnormals[j+2] = nindexes[i+2];
    }
    len = j - 3;
    indices.reserve(len);
    for (i = 0; i < len; ++i)
    {
        indices.push_back({fvertex[i]-1, ftexture[i]-1, fnormals[i]-1});
    }
}

void obj_file::open(const char *path)
{
    if (m_initialized) return;
    std::ifstream file(path, std::ios::in | std::ios::binary);
    while (file.peek() != -1)
    {
        char stype[16] = {0};
        file.getline(stype, 16, ' ');
        const auto ret = objtypes_map[stype];
        constexpr size_t max_strl = 1024;
        // print_ret(ret);
        char str[max_strl] = {0};
        file.getline(str, max_strl, '\x0a');
        switch (ret) {
            case comment:
            case mtllib:
            case smooth_shading:
            case usemtl:
            case line:
                break;
            case object_name:
                optrs.push_back(indices.size());
                // printf(">%s<\n", str);
                // printf("%i\n", optrs[str]);
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
                get_faces_index(str);
            } break;
        }
    }
    file.close();
    m_initialized = true;
}

void obj_file::draw_mesh()
{
    glBegin(GL_TRIANGLES);
    for (auto begin = indices.data(), end = begin + indices.size(); begin < end; begin++)
    {
        auto &id = *begin;
        auto &vec = vcoords[id.vertex];
        glVertex3f(vec.x, vec.y, vec.z);
        auto &v2 = vtexture[id.texture];
        glTexCoord2f(v2.x, v2.y);
        auto &normal = vnormal[id.normal];
        glNormal3f(normal.x, normal.y, normal.z);
    }
    glEnd();
}

void obj_file::draw_mesh(obj_file::iter &begin, obj_file::iter &end)
{
    glBegin(GL_TRIANGLES);
    // auto iptr = indices.data();
    for (auto current = begin; current < end; current++)
    {
        auto &id = *current;
        auto &vec = vcoords[id.vertex];
        glVertex3f(vec.x, vec.y, vec.z);
        auto &v2 = vtexture[id.texture];
        glTexCoord2f(v2.x, v2.y);
        auto &normal = vnormal[id.normal];
        glNormal3f(normal.x, normal.y, normal.z);
    }
    glEnd();
}

obj_file::iter obj_file::get_iter(unsigned int index)
{
    return indices.data() + optrs[index];
}

    //unsigned int m_idx;
// };

#ifdef READER_TEST

int main()
{
    obj_file file("objs/cuboid.obj");

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
        if (vec.vertex != test[i]-1) puts("inconsistent value"), error_count += 1;
        printf("%2i ", vec.vertex + 1);
    }
    puts("");

    for (auto &vec: file.vcoords)
    {
        printf("%9f %9f %9f\n", vec.x, vec.y, vec.z);
    }

    float vertexes[] = {1, 1, -1, 1, -1, -1, 1, 1, 1, 1, -1, 1, -1, 1, -1, -1, -1, -1, -1, 1, 1, -1, -1, 1, 0, 1, 1, 1, 1, 0, -1, 1, 0, 0, 1, -1, 0, 2.575958, 0};

    for (int i = 0, j = 0, max = file.vcoords.size(); i < max; i++, j+=3)
    {
        auto &vec = file.vcoords[i];

        if (vertexes[j] != vec.x || vertexes[j + 1] != vec.y || vertexes[j + 2] != vec.z)
        {
            puts("inconsistent value");
            error_count += 1;
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
        }
        printf("%f %f | %f %f\n", textures[j], textures[j+1], vec.x, vec.y);
    }

    float normals[] = {-0.5263, 0.6679, 0.5263, 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.5263, 0.6679, 0.5263, 0.5263, 0.6679, -0.5263, -0.5263, 0.6679, -0.5263};
    for (int i = 0, j = 0, max = file.vnormal.size(); i < max; ++i, j+=3)
    {
        auto &vec = file.vnormal[i];
        if (normals[j] != vec.x || normals[j+1] != vec.y || normals[j+2] != vec.z)
        {
            puts("inconsistent value");
            error_count += 1;
        }
        printf("%9f %9f %9f | %9f %9f %9f\n", normals[j], normals[j+1], normals[j+2], vec.x, vec.y, vec.z);
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

    return 0;
}

#endif