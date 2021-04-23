#include "pch.h"

int main()
{
    std::ifstream file("objs/object export.obj");

    puts("worked");

    file.close();

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