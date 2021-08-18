#pragma once

#include "../../vendor/include/easylogging++.hpp"
#include "../../vendor/include/json.hpp"
#include "Mesh.hpp"
#include "FileManager.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Core
{
    class Loader
    {
        private:
            nlohmann::json              _json;
            std::string                 _filepath;
            std::vector<unsigned char>  _binaryData;

            std::vector<float> GetFloats(const nlohmann::json& accessor);
            std::vector<unsigned int> GetIndices(const nlohmann::json& accessor);
            static std::vector<glm::vec2> PutFloatsIntoVec2(const std::vector<float>& floats);
            static std::vector<glm::vec3> PutFloatsIntoVec3(const std::vector<float>& floats);
            static std::vector<glm::uvec3> PutUintsIntoUvec3(const std::vector<unsigned int>& uints);
            void GetBinaryData();
            void LoadMesh(Mesh* mesh);

        public:
            void GLTFToMesh(const std::string& filepath, Mesh* mesh);
    };
}