#include "FileManager.hpp"

namespace Engine
{
    // ----- Public -----

    std::string FileManager::PadString(const std::string& string, const size_t num, const char paddingChar)
    {
        if(num > string.size())
        {
            std::string newString = string;
            //newString.insert(0, num - string.size(), paddingChar);
            newString.append(num - string.size(), paddingChar);

            return newString;
        }

        return string;
    }
    
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

            std::string fileInfo = filepath + ", Size: " + std::to_string(content.size());
            Logger::Info("Loaded", "File", fileInfo);

            return content;
        }
        else
        {
            Logger::Error("Failed", "File-Load.", filepath);

            return "FILE_ERROR";
        }
    }    
}