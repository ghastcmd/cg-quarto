#include "pch.h"
#include "vec.hpp"
#include "reader.hpp"
#include "window.hpp"
#include "stb_image.h"

#include "enum_types.hpp"
#include "hash_array.hpp"

std::ofstream log_file("./logs/log.txt");

// std::ofstream log_file;

// struct unique_test 
// {
//     unique_test(const char *path)
//     {
//         log_file.rdbuf()->pubsetbuf(0, 0);
//         log_file.open(path);
//     }
// };

// unique_test ttt("./logs/log.txt");

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

std::string get_string_ret(objtypes ret)
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

    return std::string(objtypes_str[(uint32_t)ret]);
}

#ifdef OLD

static constexpr std::pair<std::string_view, objtypes> const objtypes_array[]
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

static std::unordered_map<std::string_view, objtypes> objtypes_map (std::begin(objtypes_array), std::end(objtypes_array));

#endif

// static constexpr std::array<objtypes, std::numeric_limits<uint16_t>::max()> objtypes_array_map;

// static constexpr std::array<objtypes, 30324> objtypes_array_map {objtypes::invalid};

template <typename _ty>
struct hash_func
{
    inline constexpr _ty operator()(const char *in_str) const noexcept
    {
        return in_str[0] << 8 | in_str[1];
    }
};

DECLARE_ARRAY_MAP(objtypes_array_map, 11, objtypes, std::uint16_t, hash_func, 
    array_map::pair{"mt", objtypes::mtllib},
    array_map::pair{"#\0", objtypes::comment},
    array_map::pair{"o\0", objtypes::object_name},
    array_map::pair{"v\0", objtypes::vertex_coord},
    array_map::pair{"vn", objtypes::vertex_normal},
    array_map::pair{"vt", objtypes::vertex_texture},
    array_map::pair{"us", objtypes::usemtl},
    array_map::pair{"f\0", objtypes::face},
    array_map::pair{"l\0", objtypes::line},
    array_map::pair{"s\0", objtypes::smooth_shading},
    array_map::pair{"\n\0", objtypes::new_line}
)

// constexpr array_map::hash_array<std::uint16_t, hash_func, 11, objtypes> objtypes_array_map(
//     array_map::pair{"mt", objtypes::mtllib},
//     array_map::pair{"#\0", objtypes::comment},
//     array_map::pair{"o\0", objtypes::object_name},
//     array_map::pair{"v\0", objtypes::vertex_coord},
//     array_map::pair{"vn", objtypes::vertex_normal},
//     array_map::pair{"vt", objtypes::vertex_texture},
//     array_map::pair{"us", objtypes::usemtl},
//     array_map::pair{"f\0", objtypes::face},
//     array_map::pair{"l\0", objtypes::line},
//     array_map::pair{"s\0", objtypes::smooth_shading},
//     array_map::pair{"\n\0", objtypes::new_line}
// );

// static constexpr array_map::init::array_type_objtypes objtypes_array_map = array_map::init::init_array_map_objtypes();

void strmvcnt(char *str, int str_len, int move_count)
{
    char *endp = str + str_len;
    while (endp-- != str)
        *(endp) = *(str + move_count);
}

void revstrmove(char *str, int len, int count)
{
    char *endp = str + len + 1;
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

std::string merge_path_name(const char *path, std::string_view str)
{
    int path_size = -1;
    for (int i = strlen(path); i > 0; i--)
    {
        if (path[i] == '/' || path[i] == '\\')
        {
            path_size = i + 1;
            break;
        }
    }
    if (path_size == -1)
    {
        perror("Could not find a '\\' or '/' in path");
        return {""};
    }

    std::string ret_string {std::string_view{ path, (std::size_t)path_size }};
    ret_string += str;
    // str.insert(0, path, path_size);
    return { ret_string };
}

void dump_str(char *str)
{
    char c = 0;
    while ((c = *str++))
    {
        printf("%02x ", c);
    }
    puts("");
}

vec3 stovec3(char * str)
{
    float x = atof(str);
    while (!isspace(*str++));
    float y = atof(str++);
    while (!isspace(*str++));
    float z = atof(str);
    return {x, y, z};
}

vec3 stovec3(std::string_view str)
{
    char *end;
    float x = std::strtof(str.data(), &end);
    float y = std::strtof(end+1, &end);
    float z = std::strtof(end+1, nullptr);

    return {x, y, z};
}

// ! also deprecated (passes thru string 2x)
[[deprecated]]
vec3 stovec3__(std::string &str)
{
    const char *acstr = str.c_str();
    const char *cstr = acstr;
    uint_fast8_t count = 0;
    uint_fast8_t indexes[1] {0};
    for (uint_fast8_t i = 0; count < 2; ++i)
    {
        if (*++acstr == ' ')
        {
            indexes[count++] = i+1;
        }
    }

    float x = atof(cstr);
    float y = atof(cstr + indexes[0]);
    float z = atof(cstr + indexes[1]);

    return {x, y, z};
}

// ! Deprecated (was slow)
[[deprecated]]
vec3 stovec3_(std::string &str)
{
    size_t size1 = str.find_first_of(" ");
    size_t size2 = str.find_first_of(" ", size1 + 1);
    const char *cstr = str.c_str();
    float x = atof(cstr);
    float y = atof(cstr + size1);
    float z = atof(cstr + size2);

    return {x, y, z};
}

void stovec2(vec2& vec, char * str)
{
    vec.x = atof(str);
    while(!isspace(*str)) str++;
    vec.y = atof(str);
}

vec2 stovec2(std::string_view str)
{
    char *end;
    float x = std::strtof(str.data(), &end);
    float y = std::strtof(end+1, nullptr);

    return {x, y};
}

// ! do not use this (slow)
[[deprecated]]
vec2 stovec2_(std::string &str)
{
    size_t size = str.find_first_of(" ");
    float x = atof(str.c_str());
    float y = atof(str.c_str() + size);

    return {x, y};
}

void stof(float &val, char * str)
{
    val = atof(str);
}

float stof(std::string &str)
{
    return atof(str.c_str());
}

void stoi(unsigned int &val, char * str)
{
    val = atoi(str);
}

size_t stoi(std::string &str)
{
    size_t size;
    sscanf(str.c_str(), "%zu", &size);
    return size;
}

obj_file::obj_file(const char *path)
{
    if (m_initialized) return;
    open(path);
}

char * take_tuple(uint32_t &v, uint32_t &t, uint32_t &n, const char * str)
{
    char *end;
    v = std::strtol(str, &end, 10);
    t = std::strtol(end+1, &end, 10);
    n = std::strtol(end+1, &end, 10);
    
    return end;
}

char * take_tuple_old(uint32_t &v, uint32_t &t, uint32_t &n, const char *str)
{
    char *end;
    v = std::strtol(str, &end, 10);
    t = std::strtol(end+1, &end, 10);
    n = std::strtol(end+1, &end, 10);
    
    return end;
}

[[deprecated]]
char * take_tuple_(unsigned int &v, unsigned int &t, unsigned int &n, char *str)
{
    v = atoi(str);
    while (*str != '/') str++;
    t = atoi(++str);
    while (*str != '/') str++;
    n = atoi(++str);
    while (*str && *str != ' ') str++;

    return str;
}

void obj_file::get_faces_index(std::string_view str, std::size_t num_index)
{
    constexpr size_t buffer_size = 40;
    unsigned int vindexes[buffer_size] = {0};
    unsigned int tindexes[buffer_size] = {0};
    unsigned int nindexes[buffer_size] = {0};
    int i = 0, j = 0;

    // for (const char *stri = str.data(); *stri; ++i)
    // {
    //     printf("'%c'", *stri);
    //     stri = take_tuple(vindexes[i], tindexes[i], nindexes[i], stri);
    // }
    // log_file << "index: " << num_index << '\n';
    for (const char *stri = str.data(), *end = str.data() + (str.end() - str.begin()); stri < end; ++i)
    {
        // log_file << '\'' << std::string_view{stri, (std::size_t)(end - stri)} << "'\n";
        stri = take_tuple(vindexes[i], tindexes[i], nindexes[i], stri);
    }
    // log_file << "end of first for\n";

    int len = i;
    unsigned int fvertex [buffer_size * 4] = {0};
    unsigned int ftexture[buffer_size * 4] = {0};
    unsigned int fnormals[buffer_size * 4] = {0};
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
      //! mostly it'll be a wrong triangulation, but
      //! it makes opengl don't get errors
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
        indices.emplace_back(index{fnormals[i] - 1, ftexture[i] - 1, fvertex[i] - 1});
    }
    // log_file << "end of face index function\n";
}

std::ifstream get_file(const char *path)
{
    return std::ifstream{path, std::ios::in | std::ios::ate | std::ios::binary};
}

static objtypes get_type_from_buffer(std::vector<char> &buffer, std::size_t &index)
{
    std::size_t start = index;
    while (buffer[index] != ' ')
    {
        ++index;
    }
    const std::size_t size = index++ - start;
    std::string_view sliced {&buffer[start], size};
#ifdef OLD
    return { objtypes_map[sliced] };
#else
    char hash_key[2] {0};
    hash_key[0] = buffer[start];
    hash_key[1] = size == 1 ? '\0' : buffer[start + 1];
    return { objtypes_array_map[hash_key] };
    return objtypes::comment;
#endif
}

static std::string_view get_line_from_buffer(std::vector<char> &buffer, std::size_t &index)
{
    std::size_t start = index;
    while (buffer[index] != '\n')
    {
        ++index;
    }
    return {&buffer[start], index - start - 1};
}

void obj_file::open(const char *path)
{
    if (m_initialized)
        return;
    std::cout << path << '\n';
    // log_file << path << '\n';
    // std::ifstream file(path, std::ios::in);
    auto file { get_file(path) };
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (size == 0)
    {
        std::cerr << "Failed to locate file: " << path << '\n';
        return;
    }

    std::vector<char> buffer(size);
    file.read(buffer.data(), size); // reading file into buffer

    std::size_t num_index = 0;

    std::string_view str;
    for (std::size_t i = 0; i < size;)
    {
        // log_file << "before parsing start -- index, size: " << i << ' ' << size << '\n';
        // std::getline(file, str, ' ');
        // const auto ret = objtypes_map[str];
        // log_file << "before getting ret\n";
        const objtypes ret { get_type_from_buffer(buffer, i) };
        // log_file << "before getting line\n";
        const std::string_view str { get_line_from_buffer(buffer, i) };
        // std::getline(file, str, '\n');
        // char *str = &stri[0];
        // log_file << get_string_ret(ret) << " '" << str << "'\n";
        switch (ret) {
            case objtypes::invalid:
            case objtypes::new_line:
            case objtypes::comment:
            case objtypes::smooth_shading:
            case objtypes::object_name:
            case objtypes::line:
            break;
            case objtypes::usemtl:
            {
                // log_file << "start usemtl\n";
                auto nuevo = indices.size();
                size_t index = mat_lib.map_material[std::string(str)];
                const auto new_group = faces_group({nuevo, 0, index});
                grouping.emplace_back(new_group);
                if (const int64_t index = grouping.size() - 2; index >= 0)
                {
                    auto &pend = grouping[index];
                    pend.end = nuevo;
                }
                // log_file << "end usemtl\n";
            }
            break;
            case objtypes::mtllib: {
                // log_file << "start mtllib\n";
                std::string path_str = merge_path_name(path, str);
                mat_lib.open(path_str);
                // log_file << "end mtllib\n";
            } break;
            case objtypes::vertex_coord: {
                // vec3 vec;
                // stovec3(vec, str);
                // log_file << "start vertex_coord\n";
                vcoords.emplace_back(stovec3(str));
                // log_file << "end vertex_coord\n";
            } break;
            case objtypes::vertex_texture: {
                // vec2 vec;
                // stovec2(vec, str);
                // log_file << "start vertex_texture\n";
                vtexture.emplace_back(stovec2(str));
                // log_file << "end vertex_texture\n";
            } break;
            case objtypes::vertex_normal: {
                // vec3 vec;
                // stovec3(vec, str);
                // log_file << "start vertex_normal\n";
                vnormal.emplace_back(stovec3(str));
                // log_file << "end vertex_normal\n";
            } break;
            case objtypes::face: {
                // log_file << "start face\n";
                get_faces_index(str, num_index++);
                // log_file << "end face\n";
            } break;
        }

        while (isspace(buffer[i]))
        {
            ++i;
            if (i > size) break;
        }
    }
    // log_file << "ended file parsing\n";
    grouping[grouping.size()-1].end = indices.size();
    m_initialized = true;
    // log_file << "end of obj_file::open function\n";
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

void obj_file::draw_mesh(const obj_file::iter &begin, const obj_file::iter &end)
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

void obj_file::draw_mat_mesh()
{
    for (auto &group: grouping)
    {
        if (mat_lib.init())
        mat_lib.materials[group.mat_index].apply_material();

        iter left = indices.data() + group.begin;
        iter right = indices.data() + group.end;
        // draw_mesh(group.begin, group.end);
        draw_mesh(left, right);
    }
}

void texture::open(const char *path)
{
    if (m_init)
    {
        puts("Already initialized texture object");
        return;
    }
    stbi_set_flip_vertically_on_load(1);
    m_local_buffer = stbi_load(path, &m_width, &m_height, &m_nr_channels, 0);
    if (!m_local_buffer)
    {
        printf("Could not open requested file %s\n", path);
        return;
    }
    m_init = true;

    glGenTextures(1, &m_texture_id);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);


    const bool not_div_by_four = m_width % 4 != 0;
    if (not_div_by_four)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_local_buffer);        
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_local_buffer);
    }

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

static constexpr std::pair<const char*, mtltypes> const mtltypes_array[]
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
    if (!file.is_open())
    {
        printf("Could not open the mtl file %s\n", path);
        file.close();
        return;
    }
    // Here I use a dummy material, but is guaranteed that it isn't used
    // material dummy_mat {{0}, {0}, {0}, {0}, 0};
    // material &current_mat = dummy_mat;
    std::string str;
    while (file.peek() != -1)
    {
        // char stype[16] = {0};
        while (file.peek() == '\n') file.get();
        // file >> str;
        std::getline(file, str, ' ');
        // file.getline(stype, std::size(stype), ' ');
        const auto ret = mtltypes_map[str];
        // char str[64] = {0};
        // file.getline(str, std::size(str));
        // file.getline(stri);
        std::getline(file, str, '\n');
        switch (ret) {
            case mtltypes::invalid:
            case mtltypes::inverse_dissolve:
            case mtltypes::new_line:
            case mtltypes::transmission_filter:
            break;
            case mtltypes::comment:
                if (char *tstr = &str[0]; !strncmp(str.c_str(), "Material", std::size("Material") - 1))
                {
                    size_t last_space = str.find_last_of(" ");
                    m_material_len = atoi(tstr + last_space);
                    materials.reserve(m_material_len);
                    map_material.reserve(m_material_len);
                }
            break;
            case mtltypes::new_material:
                mat_names.push_back(str);
                m_ci++;
                map_material[str] = m_ci;
                materials.emplace_back(material{{0}, {0}, {0}, {0}, 0});
            break;
            case mtltypes::ambient:
                materials[m_ci].ambient = stovec3(str);
            break;
            case mtltypes::diffuse:
                materials[m_ci].diffuse = stovec3(str);
            break;
            case mtltypes::specular:
                materials[m_ci].specular = stovec3(str);
            break;
            case mtltypes::shininess:
                materials[m_ci].highlights = atof(str.c_str());
            break;
            case mtltypes::dissolve:
                materials[m_ci].dissolve = stof(str);
            break;
            case mtltypes::optical_density:
                materials[m_ci].optical_density = stof(str);
            break;
            case mtltypes::illumination_mode:
                materials[m_ci].illum_model = stof(str);
            break;
            case mtltypes::map_diffuse: {
                std::string path_str = merge_path_name(path, str);
                materials[m_ci].tex_diffuse.open(path_str);
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
    m_init = true;
}