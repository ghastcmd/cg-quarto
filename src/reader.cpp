#include "pch.h"
#include "vec.hpp"
#include "reader.hpp"
#include "window.hpp"

enum class objtypes : unsigned int
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
    {"#", objtypes::comment},
    {"mtllib", objtypes::mtllib},
    {"o", objtypes::object_name},
    {"v", objtypes::vertex_coord},
    {"vn", objtypes::vertex_normal},
    {"vt", objtypes::vertex_texture},
    {"usemtl", objtypes::usemtl},
    {"f", objtypes::face},
    {"l", objtypes::line}
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

    puts(objtypes_str[(unsigned int)ret]);
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
        indices.push_back({fnormals[i]-1, ftexture[i]-1, fvertex[i]-1});
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
            case objtypes::comment:
            case objtypes::mtllib:
            case objtypes::smooth_shading:
            case objtypes::usemtl:
            case objtypes::line:
                break;
            case objtypes::object_name:
                optrs.push_back(indices.size());
            break;
            case objtypes::vertex_coord: {
                vec3 vec;
                stovec3(vec, str);
                vcoords.push_back(vec);
            } break;
            case objtypes::vertex_texture: {
                vec2 vec;
                stovec2(vec, str);
                vtexture.push_back(vec);
            } break;
            case objtypes::vertex_normal: {
                vec3 vec;
                stovec3(vec, str);
                vnormal.push_back(vec);
            } break;
            case objtypes::face: {
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
        auto &normal = vnormal[id.normal];
        glNormal3f(normal.x, normal.y, normal.z);
        auto &v2 = vtexture[id.texture];
        glTexCoord2f(v2.x, v2.y);
        auto &vec = vcoords[id.vertex];
        glVertex3f(vec.x, vec.y, vec.z);
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
        auto &normal = vnormal[id.normal];
        glNormal3f(normal.x, normal.y, normal.z);
        auto &v2 = vtexture[id.texture];
        glTexCoord2f(v2.x, v2.y);
        auto &vec = vcoords[id.vertex];
        glVertex3f(vec.x, vec.y, vec.z);
    }
    glEnd();
}

obj_file::iter obj_file::get_iter(unsigned int index)
{
    return indices.data() + optrs[index];
}

void material::apply_material() const
{
    const float fambient[] {ambient.x, ambient.y, ambient.z, 1.0f};
    const float fdiffuse[] {diffuse.x, diffuse.y, diffuse.z, 1.0f};
    const float femission[] {emissive.x, emissive.y, emissive.z, 1.0f};
    const float fspecular[] {specular.x, specular.y, specular.z, 1.0f};
    const float fshininess[] {highlights};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, fambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, fdiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, fspecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, femission);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, highlights);
}

enum class mtltypes : unsigned int
{
    invalid,
    comment,
    new_material,
    ambient,
    diffuse,
    specular,
    shininess,
    dissolve,
    inverse_dissolve,
    transmission_filter,
    optical_density,
    illumination_mode,
    map_ambient,
    map_diffuse,
    map_specular,
    map_highlight,
    map_alpha,
    map_bump,
    map_displacement
};

static std::unordered_map<const char*, mtltypes> mtltypes_map
{
    {"#", mtltypes::comment},
    {"newmtl", mtltypes::new_material},
    {"Ka", mtltypes::ambient},
    {"Kd", mtltypes::diffuse},
    {"Ks", mtltypes::specular},
    {"Ns", mtltypes::shininess},
    {"d", mtltypes::dissolve},
    {"Tr", mtltypes::inverse_dissolve},
    {"Tf", mtltypes::transmission_filter},
    {"Ni", mtltypes::optical_density},
    {"illum", mtltypes::illumination_mode},
    {"map_Ka", mtltypes::map_ambient},
    {"map_Kd", mtltypes::map_diffuse},
    {"map_Ks", mtltypes::map_specular},
    {"map_Ns", mtltypes::map_highlight},
    {"map_d", mtltypes::map_alpha},
    {"map_bump", mtltypes::map_bump},
    {"bump", mtltypes::map_bump},
    {"disp", mtltypes::map_displacement},
};

mtl_file::mtl_file(const char *path)
{
    // if (m_initialized) return;
    std::ifstream file(path, std::ios::in | std::ios::binary);
    while (file.peek() != -1)
    {
        char stype[16] = {0};
        file.getline(stype, 16, ' ');
        // const auto ret = objtypes_map[stype];
        const auto ret = mtltypes_map[stype];
        constexpr size_t max_strl = 64;
        // print_ret(ret);
        char str[max_strl] = {0};
        file.getline(str, max_strl, '\x0a');
        switch (ret) {
            case mtltypes::comment:
            case mtltypes::new_material:
            case mtltypes::ambient:
            case mtltypes::diffuse:
            case mtltypes::specular:
            case mtltypes::shininess:
            case mtltypes::dissolve:
            case mtltypes::transmission_filter:
            case mtltypes::optical_density:
            case mtltypes::illumination_mode:
            case mtltypes::map_ambient:
            case mtltypes::map_diffuse:
            case mtltypes::map_specular:
            case mtltypes::map_highlight:
            case mtltypes::map_alpha:
            case mtltypes::map_bump:
            case mtltypes::map_displacement:
            break;
        }
    }
    file.close();
}