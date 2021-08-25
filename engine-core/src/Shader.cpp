#include "Shader.hpp"

namespace Core
{
    // ----- Private -----

    int32 Shader::Compile(uint32 shaderType, const std::string& source)
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
            LOG(ERROR) << "Failed to compile shader: " << shaderType;
            LOG(ERROR) << "Error: " << std::string(msg.get());

            //Free resources and return
            GLCall(glDeleteShader(id));
            return -1;
        }

        LOG(INFO) << "Successfully compiled shader: " << shaderType;
        return id;
    }

    int32 Shader::Build(uint32 vsID, uint32 fsID)
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
            LOG(ERROR) << "Failed to link shader: " << programID;
            LOG(ERROR) << "Error: " << std::string(msg.get());

            //Free resources and return
            GLCall(glDeleteProgram(programID));
            return -1;
        }

        GLCall(glValidateProgram(programID));

        //Free shader
        GLCall(glDetachShader(programID, vsID));
        GLCall(glDetachShader(programID, fsID));
        GLCall(glDeleteShader(vsID));
        GLCall(glDeleteShader(fsID));

        LOG(INFO) << "Successfully linked shader to program: " << programID;
        return programID;
    }

    int32 Shader::Create(const std::string& vsFilepath, const std::string& fsFilepath)
    {
        //Read in shader files
        std::string vsSource = FileManager::FileToString(vsFilepath);
        std::string fsSource = FileManager::FileToString(fsFilepath);

        if(vsSource == "FILE_ERROR" || fsSource == "FILE_ERROR")
            return -1;

        //Compile shader
        int32 vsID = Compile(GL_VERTEX_SHADER, vsSource);
        int32 fsID = Compile(GL_FRAGMENT_SHADER, fsSource);

        if(vsID < 0 || fsID < 0)
            return -2;

        //Build shader into program
        int32 programID = Build(vsID, fsID);

        if(programID < 0)
            return -3;

        return programID;    
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
    {
        _shaderID = Create(vsFilepath, fsFilepath);
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
}