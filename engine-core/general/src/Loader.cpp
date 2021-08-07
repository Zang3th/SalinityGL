#include "Loader.hpp"

namespace Core
{
    // ----- Private -----

    std::vector<float> Loader::GetFloats(const nlohmann::json& accessor)
    {
        std::vector<float> floats;

        //Get properties from the accessor
        unsigned int bufferViewIndex = accessor.value("bufferView", 1);
        unsigned int count = accessor["count"];
        unsigned int byteOffsetAccessor = accessor.value("byteOffset", 0);
        std::string type = accessor["type"];

        //Get properties from the bufferView
        nlohmann::json bufferView = _json["bufferViews"][bufferViewIndex];
        unsigned int byteOffset = bufferView["byteOffset"];

        //Interpret the type and store it into numPerVert
        unsigned int numPerVert;
        if (type == "SCALAR") numPerVert = 1;
        else if (type == "VEC2") numPerVert = 2;
        else if (type == "VEC3") numPerVert = 3;
        else if (type == "VEC4") numPerVert = 4;
        else throw std::invalid_argument("Type is invalid (not SCALAR, VEC2, VEC3, or VEC4)");

        //Go over all the bytes in the data at the correct place using the properties from above
        unsigned int beginningOfData = byteOffset + byteOffsetAccessor;
        unsigned int lengthOfData = count * 4 * numPerVert;
        float value;
        for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i += 4)
        {
            unsigned char bytes[] = { _binaryData[i], _binaryData[i + 1], _binaryData[i + 2], _binaryData[i + 3] };
            std::memcpy(&value, bytes, sizeof(float));
            floats.push_back(value);
        }

        return floats;
    }

    std::vector<unsigned int> Loader::GetIndices(const nlohmann::json& accessor)
    {
        std::vector<unsigned int> indices;

        //Get properties from the accessor
        unsigned int bufferViewIndex = accessor.value("bufferView", 0);
        unsigned int count = accessor["count"];
        unsigned int byteOffsetAccessor = accessor.value("byteOffset", 0);
        unsigned int componentType = accessor["componentType"];

        //Get properties from the bufferView
        nlohmann::json bufferView = _json["bufferViews"][bufferViewIndex];
        unsigned int byteOffset = bufferView["byteOffset"];

        //Get indices in regard to their type: unsigned int, unsigned short, or short
        unsigned int beginningOfData = byteOffset + byteOffsetAccessor;
        if (componentType == 5125)
        {
            for (unsigned int i = beginningOfData; i < byteOffset + byteOffsetAccessor + count * 4; i += 4)
            {
                unsigned char bytes[] = { _binaryData[i], _binaryData[i + 1], _binaryData[i + 2], _binaryData[i + 3] };
                unsigned int value;
                std::memcpy(&value, bytes, sizeof(unsigned int));
                indices.push_back((unsigned int)value);
            }
        }
        else if (componentType == 5123)
        {
            for (unsigned int i = beginningOfData; i < byteOffset + byteOffsetAccessor + count * 2; i += 2)
            {
                unsigned char bytes[] = { _binaryData[i], _binaryData[i + 1] };
                unsigned short value;
                std::memcpy(&value, bytes, sizeof(unsigned short));
                indices.push_back((unsigned int)value);
            }
        }
        else if (componentType == 5122)
        {
            for (unsigned int i = beginningOfData; i < byteOffset + byteOffsetAccessor + count * 2; i += 2)
            {
                unsigned char bytes[] = { _binaryData[i], _binaryData[i + 1] };
                short value;
                std::memcpy(&value, bytes, sizeof(short));
                indices.push_back((unsigned int)value);
            }
        }

        return indices;
    }

    std::vector<glm::vec2> Loader::PutFloatsIntoVec2(const std::vector<float>& floats)
    {
        std::vector<glm::vec2> vector;
        for (unsigned int i = 0; i < floats.size(); i += 2)
        {
            vector.emplace_back(glm::vec2(floats[i], floats[i + 1]));
        }
        return vector;
    }

    std::vector<glm::vec3> Loader::PutFloatsIntoVec3(const std::vector<float>& floats)
    {
        std::vector<glm::vec3> vector;
        for (unsigned int i = 0; i < floats.size(); i += 3)
        {
            vector.emplace_back(glm::vec3(floats[i], floats[i + 1], floats[i + 2]));
        }
        return vector;
    }

    std::vector<glm::uvec3> Loader::PutUintsIntoUvec3(const std::vector<unsigned int>& uints)
    {
        std::vector<glm::uvec3> vector;
        for (unsigned int i = 0; i < uints.size(); i += 3)
        {
            vector.emplace_back(glm::uvec3(uints[i], uints[i + 1], uints[i + 2]));
        }
        return vector;
    }

    void Loader::GetBinaryData()
    {
        //Get uri of the .bin file
        std::string uri = _json["buffers"][0]["uri"];

        //Store .bin file into a string
        std::string fileDirectory = _filepath.substr(0, _filepath.find_last_of('/') + 1);
        std::string binAsString = FileManager::FileToString(fileDirectory + uri);

        //Put string into byte buffer
        _binaryData = std::vector<unsigned char>(binAsString.begin(), binAsString.end());
    }

    void Loader::LoadMesh(Mesh* mesh)
    {
        //Get all accessor indices
        unsigned int posAccInd = _json["meshes"][0]["primitives"][0]["attributes"]["POSITION"];
        unsigned int normalAccInd = _json["meshes"][0]["primitives"][0]["attributes"]["NORMAL"];
        unsigned int texAccInd = _json["meshes"][0]["primitives"][0]["attributes"]["TEXCOORD_0"];
        unsigned int indAccInd = _json["meshes"][0]["primitives"][0]["indices"];

        //Use accessor indices to get vertice components
        std::vector<float> posAsFloats = GetFloats(_json["accessors"][posAccInd]);
        std::vector<float> normalAsFloats = GetFloats(_json["accessors"][normalAccInd]);
        std::vector<float> texAsFloats = GetFloats(_json["accessors"][texAccInd]);
        std::vector<unsigned int> indices = GetIndices(_json["accessors"][indAccInd]);

        std::vector<glm::vec3> positions = PutFloatsIntoVec3(posAsFloats);
        std::vector<glm::vec3> normals = PutFloatsIntoVec3(normalAsFloats);
        std::vector<glm::vec2> texUVs = PutFloatsIntoVec2(texAsFloats);
        std::vector<glm::uvec3> indVec = PutUintsIntoUvec3(indices);

        //Save data into mesh
        mesh->vertices = positions;
        mesh->indices = indVec;
        mesh->normals = normals;
        mesh->texCoords = texUVs;
    }

    // ----- Public -----

    void Loader::GLTFToMesh(const std::string& filepath, Mesh* mesh)
    {
        //Save filepath
        _filepath = filepath;

        //Create json object out of text file
        std::string file = FileManager::FileToString(_filepath);
        _json = nlohmann::json::parse(file);

        //Get binary data
        GetBinaryData();

        //Load mesh
        LoadMesh(mesh);
    }
}