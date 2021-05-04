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
    iter get_iter(char *object_name);

    bool m_initialized = false;

    std::vector<vec3> vcoords;
    std::vector<vec3> vnormal;
    std::vector<vec2> vtexture;

    std::vector<index> indices;
    std::unordered_map<char *, iter> optrs;
private:
    void get_faces_index(char *str);
};
