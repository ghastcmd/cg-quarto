#pragma once

// #define READER_TEST
// #define TEST

struct obj_file
{
    struct index
    {
        unsigned int vertex, texture, normal;
    };

    using iter = std::vector<index>::pointer;

    obj_file() = default;
    obj_file(const char *path);
    void open(const char *path);

    void draw_mesh();
    void draw_mesh(iter &begin, iter &end);
    iter get_iter(unsigned int index);

    bool m_initialized = false;

    std::vector<vec3> vcoords;
    std::vector<vec3> vnormal;
    std::vector<vec2> vtexture;

    std::vector<index> indices;
    // std::unordered_map<std::string_view, unsigned int> optrs;
    std::vector<unsigned int> optrs;
private:
    void get_faces_index(char *str);
};

struct material
{
    material(vec3 ambient, vec3 diffuse, vec3 specular, vec3 emissive, float shininess)
        : ambient(ambient), diffuse(diffuse), specular(specular), 
        emissive(emissive), highlights(shininess)
    {}

    void apply_material() const;

    vec3 ambient, diffuse, specular, emissive;
    float highlights, optical_density, dissolve;
    unsigned int illum_model;
    std::string diffuse_map;
};