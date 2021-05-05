#pragma once

// #define READER_TEST
// #define TEST

struct obj_file
{
    obj_file() = default;
    obj_file(const char *path);
    void open(const char *path);

    void draw_mesh();

    bool m_initialized = false;

    std::vector<vec3> vcoords;
    std::vector<vec3> vnormal;
    std::vector<vec2> vtexture;

    struct index
    {
        unsigned int vertex, texture, normal;
    };

    std::vector<index> indices;
    std::vector<unsigned int> optrs;
private:
    void get_faces_index(char *str);
};

struct material
{
    vec3 ambient, diffuse, specular, emmisive;
    float highlights, optical_density, dissolve;
    unsigned int illum_model;
    std::string diffuse_map;
};