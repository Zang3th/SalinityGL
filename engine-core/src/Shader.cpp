#include "Shader.hpp"

namespace Core
{
    // ----- Private -----

    uint32 Shader::Compile(uint32 shaderType, const std::string& source)
    {
        const char* src = source.c_str();

        GLCall(uint32 id = glCreateShader(shaderType))
        GLCall(glShaderSource(id, 1, &src, nullptr));
        GLCall(glCompileShader(id));

        //Errorhandling
        int32 result;
        GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

        if(result == GL_FALSE)
        {
            //Get error message length
            int32 length;
            GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

            //Get error message
            Scope<char[]> msg(new char[length]);
            GLCall(glGetShaderInfoLog(id, length, &length, msg.get()));

            //Log error message
            LOG(ERROR) << "Failed:  Shader-Compilation | " << shaderType << " | "<< std::string(msg.get());

            //Free resources and return
            GLCall(glDeleteShader(id));
        }
        else
        {
            LOG(INFO) << "Compiled: Shader | " << shaderType;
        }

        return id;
    }

    uint32 Shader::Build(uint32 vsID, uint32 fsID)
    {
        GLCall(uint32 programID = glCreateProgram());
        GLCall(glAttachShader(programID, vsID));
        GLCall(glAttachShader(programID, fsID));
        GLCall(glLinkProgram(programID));

        //Errorhandling
        int32 result;
        GLCall(glGetProgramiv(programID, GL_LINK_STATUS, &result));
        if(!result)
        {
            //Get error message length
            int32 length;
            GLCall(glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length));

            //Get error message
            Scope<char[]> msg(new char[length]);
            GLCall(glGetProgramInfoLog(programID, length, &length, msg.get()));

            //Log error message
            LOG(ERROR) << "Failed:  Shader-Linking | " << programID << " | "<< std::string(msg.get());

            //Free resources and return
            GLCall(glDeleteProgram(programID));
        }
        else
        {
            LOG(INFO) << "Linked:   Program | " << programID;
        }

        GLCall(glValidateProgram(programID));

        //Free shader
        GLCall(glDetachShader(programID, vsID));
        GLCall(glDetachShader(programID, fsID));
        GLCall(glDeleteShader(vsID));
        GLCall(glDeleteShader(fsID));

        return programID;
    }

    uint32 Shader::Create(const std::string& vsFilepath, const std::string& fsFilepath)
    {
        //Read in shader files
        std::string vsSource = FileManager::FileToString(vsFilepath);
        std::string fsSource = FileManager::FileToString(fsFilepath);

        //Compile shader
        uint32 vsID = Compile(GL_VERTEX_SHADER, vsSource);
        uint32 fsID = Compile(GL_FRAGMENT_SHADER, fsSource);

        //Build shader into program
        return Build(vsID, fsID);
    }

    int32 Shader::GetUniformLocation(const std::string& name)
    {
        //Check if uniform is in cache, otherwise cache it and return the location
        if (_uniformLocationCache.find(name) != _uniformLocationCache.end())
                return _uniformLocationCache[name];

        GLCall(int32 location = glGetUniformLocation(_shaderID, name.c_str()));
        _uniformLocationCache[name] = location;
        return location;
    }

    // ----- Public -----

    Shader::Shader(const std::string& vsFilepath, const std::string& fsFilepath)
        :   _shaderID(Create(vsFilepath, fsFilepath))
    {

    }

    Shader::~Shader()
    {
        GLCall(glDeleteProgram(_shaderID));
    }

    void Shader::Bind() const
    {
        GLCall(glUseProgram(_shaderID));
    }

    void Shader::Unbind() const
    {
        GLCall(glUseProgram(0));
    }   

    void Shader::SetUniform1i(const std::string& name, int32 value)
    {
        GLCall(glUniform1i(GetUniformLocation(name), value));
    }

    void Shader::SetUniform1f(const std::string& name, float value)
    {
        GLCall(glUniform1f(GetUniformLocation(name), value))
    }

    void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
    {
        GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
    }

    void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
    {
        GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
    }

    void Shader::SetUniformVec3f(const std::string& name, const glm::vec3& vec)
    {
        GLCall(glUniform3fv(GetUniformLocation(name), 1, &vec[0]));
    }

    void Shader::SetUniformVec4f(const std::string& name, const glm::vec4& vec)
    {
        GLCall(glUniform4fv(GetUniformLocation(name), 1, &vec[0]));
    }

    uint32 Shader::GetShaderID() const
    {
        return _shaderID;
    }
}