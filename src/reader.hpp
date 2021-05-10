#pragma once

#define READER_TEST
#define TEST

struct texture
{
    texture() = default;
    texture(const char *path) { open(path); }
    void operator=(texture &other)
    {
        m_init = other.m_init;
        m_texture_id = other.m_texture_id;
        m_local_buffer = other.m_local_buffer;
        m_width = other.m_width;
        m_height = other.m_height;
        m_nr_channels = other.m_nr_channels;
    }

    void open(const char *path);
    ~texture();

    void bind() const;
    void unbind() const;

    bool m_init = false;
private:
    unsigned int m_texture_id = 0;
    unsigned char *m_local_buffer = nullptr;
    int m_width = 0, m_height = 0, m_nr_channels = 0;
};

struct material
{
    material(vec3 ambient, vec3 diffuse, vec3 specular, vec3 emissive, float shininess)
        : ambient(ambient), diffuse(diffuse), specular(specular), emissive(emissive),
        highlights(shininess)
    {
    }

    void apply_material() const;
    void dump_material() const;
    void zero_material();

    vec3 ambient, diffuse, specular, emissive;
    double highlights;
    float optical_density, dissolve;
    unsigned int illum_model;
    texture tex_diffuse;
};

struct mtl_file
{
    mtl_file() = default;
    mtl_file(const char *path) { open(path); }
    void open(const char *path);

    std::vector<material> materials;
    std::vector<std::string> mat_names;
    std::unordered_map<std::string, size_t> map_material;

    size_t m_material_len = 0;
    size_t m_ci = -1;
};

struct obj_file
{
    struct index
    {
        unsigned int normal, texture, vertex;
    };

    using iter = std::vector<index>::pointer;

    obj_file() = default;
    obj_file(const char *path);
    void open(const char *path);

    void draw_mesh();
    void draw_mesh(const iter &begin, const iter &end);
    void draw_mat_mesh();

    iter get_iter(unsigned int index);

    bool m_initialized = false;

    std::vector<vec3> vcoords;
    std::vector<vec3> vnormal;
    std::vector<vec2> vtexture;

    std::vector<index> indices;
    // std::unordered_map<std::string_view, unsigned int> optrs;
    std::vector<unsigned int> optrs;

    struct faces_group
    {
        size_t begin, end;
        size_t mat_index;
    };

    std::vector<faces_group> grouping;

    mtl_file mat_lib;
private:
    void get_faces_index(char *str);
};