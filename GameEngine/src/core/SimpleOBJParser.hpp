#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>
#include "RawData.hpp"

class SimpleOBJParser : public RawData
{
private:
    const char* _path;
    unsigned int _faces;
    std::vector<glm::vec3> temp_vertices;

public:
    SimpleOBJParser(const char* filepath)
        : _path(filepath)
    {
        init();
    }   

    void init() override
    {
        FILE* file = fopen(_path, "r");
        if (file == NULL) 
            printf("Impossible to open the file !\n");

        while (1) 
        {
            char lineHeader[128];
            // read the first word of the line
            int res = fscanf(file, "%s", lineHeader);
            if (res == EOF)
                break; // EOF = End Of File. Quit the loop.

            if (strcmp(lineHeader, "v") == 0) 
            {
                float x = 0, y = 0, z = 0;
                fscanf(file, "%f %f %f\n", &x, &y, &z);
                _vertices.emplace_back(glm::vec3(x, y, z));
                //std::cout << "x: " << vertex.x << " y: " << vertex.y << " z: " << vertex.z << std::endl;
            }
            else if (strcmp(lineHeader, "vt") == 0) 
            {
                float u = 0, v = 0;
                fscanf(file, "%f %f\n", &u, &v);
                _texCoords.emplace_back(glm::vec2(u, v));
            }
            else if (strcmp(lineHeader, "f") == 0) 
            {
                unsigned int a = 0, b = 0, c = 0;
                fscanf(file, "%d/%d/%d\n", &a, &b, &c);
                _indices.emplace_back(glm::uvec3(a, b, c));                
            }
        }
        setParameters();
    }     

    void setParameters() override
    {
        _verticeSize = _vertices.size() * sizeof(glm::vec3);
        _indiceSize = _indices.size() * sizeof(glm::uvec3);
        _texCoordSize = _texCoords.size() * sizeof(glm::vec2);
        _verticesToRender = (GLsizei)_indices.size() * 3;
    }
};