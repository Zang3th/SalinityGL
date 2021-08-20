#include "FileManager.hpp"

namespace Core
{
    // ----- Public -----
    
    std::string FileManager::FileToString(const std::string& filepath)
    {
        std::ifstream file(filepath);
        if(file)
        {
            std::string content;

            file.seekg(0, std::ios::end);
            content.resize(file.tellg());
            file.seekg(0, std::ios::beg);
            file.read(&content[0], content.size());
            file.close();

            LOG(INFO) << "Successfully read file at: " << filepath << " (Size: " << content.size() << ")";
            return(content);
        }
        else
        {
            LOG(ERROR) << "Could not open file: " << filepath;
            return "FILE_ERROR";
        }
    }    
}