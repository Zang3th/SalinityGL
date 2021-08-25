#include "ObjLoader.hpp"
#include <iostream>

namespace Core
{
    // ----- Public -----

    void ObjLoader::ObjToMesh(const std::string& filename, const std::string& baseFilepath, Mesh* mesh)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;
        std::string objFullFilepath = baseFilepath + "/" + filename + ".obj";

        if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, objFullFilepath.c_str(), baseFilepath.c_str()))
            LOG(ERROR) << "Failed:   Obj-File-Loading | " << objFullFilepath;
        else
            LOG(INFO) << "Loaded:   Obj-File | " << objFullFilepath << " (Shapes: " << shapes.size() << " | Materials: " << materials.size() << ")";

        //Additional error checking to catch edge cases
        if(!warn.empty())
            LOG(WARNING) << "Failed:   " << warn << " | " << objFullFilepath;
        if(!err.empty())
            LOG(ERROR) << "Failed:   " << err << " | " << objFullFilepath;
        if(shapes.size() != 1)
            LOG(ERROR) << "Failed:   More than 1 shape per file is not supported | " << objFullFilepath;
    }
}