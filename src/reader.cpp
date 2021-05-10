#include "pch.h"
#include "vec.hpp"
#include "reader.hpp"
#include "window.hpp"
#include "stb_image.h"

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
    line,
    new_line
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
        "line",
        "new_line"
    };

    puts(objtypes_str[(unsigned int)ret]);
}

static std::pair<const char*, objtypes> const objtypes_array[]
{
    {"#", objtypes::comment},
    {"mtllib", objtypes::mtllib},
    {"o", objtypes::object_name},
    {"v", objtypes::vertex_coord},
    {"vn", objtypes::vertex_normal},
    {"vt", objtypes::vertex_texture},
    {"usemtl", objtypes::usemtl},
    {"f", objtypes::face},
    {"l", objtypes::line},
    {"s", objtypes::smooth_shading},
    {"\n", objtypes::new_line}
};

static std::unordered_map<std::string, objtypes> objtypes_map (std::begin(objtypes_array), std::end(objtypes_array));

void print_val(std::ifstream& fstream)
{
    char str[16];
    fstream.getline(str, 16, '\x0a');
    puts(str);
}

void strmvcnt(char *str, int str_len, int move_count)
{
    char *endp = str + str_len;
    while (endp-- != str)
        *(endp) = *(str + move_count);
}

void revstrmove(char *str, int len, int count)
{
    char *endp = str + len;
    while (endp-- != str)
        *(endp + count) = *endp;
}

void merge_path_name(const char *path, char *name)
{
    const char *end_path = path + strlen(path);
    while(end_path >= path - 1 && *end_path-- != '/');
    int dir_len = end_path - path + 2;

    revstrmove(name, strlen(name), dir_len);
    strncpy(name, path, dir_len);
}

void dump_str(char *str)
{
    char c;
    while (c = *str++)
    {
        printf("%02x ", c);
    }
    puts("");
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

void stof(float &val, char * str)
{
    val = atof(str);
}

void stoi(unsigned int &val, char * str)
{
    val = atoi(str);
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
    std::ifstream file(path, std::ios::in);
    while (file.peek() != -1)
    {
        char stype[8] = {0};
        file.getline(stype, std::size(stype), ' ');
        const auto ret = objtypes_map[stype];
        // print_ret(ret);
        char str[1024] = {0};
        file.getline(str, std::size(str));
        switch (ret) {
            case objtypes::comment:
            case objtypes::smooth_shading:
            case objtypes::line:
            case objtypes::usemtl:
            break;
            case objtypes::mtllib:
                merge_path_name(path, str);
                mat_lib.open(str);
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

void texture::open(const char *path)
{
    if (m_init)
    {
        puts("Already initialized texture object");
        return;
    }
    m_init = true;
    stbi_set_flip_vertically_on_load(1);
    m_local_buffer = stbi_load(path, &m_width, &m_height, &m_nr_channels, 0);
    if (!m_local_buffer)
    {
        m_init = false;
        return;
    }

    glGenTextures(1, &m_texture_id);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_local_buffer);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (m_local_buffer)
    {
        stbi_image_free(m_local_buffer);
    }
}

texture::~texture()
{
    glDeleteTextures(1, &m_texture_id);
}

void texture::bind() const
{
    // glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
}

void texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}


void material::apply_material() const
{
    const float fambient[] {ambient.x, ambient.y, ambient.z, 1.0f};
    const float fdiffuse[] {diffuse.x, diffuse.y, diffuse.z, 1.0f};
    const float femission[] {emissive.x, emissive.y, emissive.z, 1.0f};
    const float fspecular[] {specular.x, specular.y, specular.z, 1.0f};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, fambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, fdiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, fspecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, femission);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, highlights);

    tex_diffuse.bind();
}

void material::dump_material() const
{
    printf("ambient: %f %f %f\n", ambient.x, ambient.y, ambient.z);
    printf("diffuse: %f %f %f\n", diffuse.x, diffuse.y, diffuse.z);
    printf("specular: %f %f %f\n", specular.x, specular.y, specular.z);
    printf("emission: %f %f %f\n", emissive.x, emissive.y, emissive.z);
    printf("highlithgts: %f\n", highlights);
    printf("optical_density: %f\n", optical_density);
    printf("dissolve: %f\n", dissolve);
    printf("illumination_mode: %i\n", illum_model);
}

void material::zero_material()
{
    ambient = {0};
    diffuse = {0};
    specular = {0};
    emissive = {0};
    highlights = {0};
    optical_density = {0};
    dissolve = {0};
    illum_model = 0;
    tex_diffuse.m_init = false;    
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
    map_displacement,
    new_line
};

void print_ret(mtltypes ret)
{
    const char *objtypes_str[] {
        "invalid",
        "comment",
        "new_material",
        "ambient",
        "diffuse",
        "specular",
        "shininess",
        "dissolve",
        "inverse_dissolve",
        "transmission_filter",
        "optical_density",
        "illumination_mode",
        "map_ambient",
        "map_diffuse",
        "map_specular",
        "map_highlight",
        "map_alpha",
        "map_bump",
        "map_displacement",
        "new_line"
    };

    puts(objtypes_str[(unsigned int)ret]);
}

static std::pair<const char*, mtltypes> const mtltypes_array[]
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
    {"\n", mtltypes::new_line}
};

static std::unordered_map<std::string, mtltypes> mtltypes_map (std::begin(mtltypes_array), std::end(mtltypes_array));

void mtl_file::open(const char *path)
{
    // if (m_init) return;
    std::ifstream file(path, std::ios::in);
    // Here I use a dummy material, but is guaranteed that it is't used
    material dummy_mat {{0}, {0}, {0}, {0}, 0};
    material &current_mat = dummy_mat;
    while (file.peek() != -1)
    {
        char stype[16] = {0};
        while (file.peek() == '\n') file.get();
        file.getline(stype, std::size(stype), ' ');
        const auto ret = mtltypes_map[stype];
        char str[64] = {0};
        file.getline(str, std::size(str));
        switch (ret) {
            case mtltypes::new_line:
            case mtltypes::transmission_filter:
            break;
            case mtltypes::comment:
                if (char *tstr = str; !strncmp(str, "Material", std::size("Material") - 1))
                {
                    while (!isspace(*tstr)) tstr++;
                    tstr++;
                    while (!isspace(*tstr)) tstr++;
                    m_material_len = atoi(tstr);
                    materials.reserve(m_material_len);
                }
            break;
            case mtltypes::new_material:
                mat_names.push_back(str);
                m_ci++;
                materials.emplace_back(material{{0}, {0}, {0}, {0}, 0});
            break;
            case mtltypes::ambient:
                stovec3(materials[m_ci].ambient, str);
            break;
            case mtltypes::diffuse:
                stovec3(materials[m_ci].diffuse, str);
            break;
            case mtltypes::specular:
                stovec3(materials[m_ci].specular, str);
            break;
            case mtltypes::shininess:
                materials[m_ci].highlights = atof(str);
            break;
            case mtltypes::dissolve:
                stof(materials[m_ci].dissolve, str);
            break;
            case mtltypes::optical_density:
                stof(materials[m_ci].optical_density, str);
            break;
            case mtltypes::illumination_mode:
                stoi(materials[m_ci].illum_model, str);
            break;
            case mtltypes::map_diffuse: {
                merge_path_name(path, str);
                materials[m_ci].tex_diffuse.open(str);
            } break;
            case mtltypes::map_ambient:
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