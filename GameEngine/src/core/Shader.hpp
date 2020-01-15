#include "OpenGLErrorManager.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

class Shader 
{
private:
	std::string _vs_Filepath;
	std::string _fs_Filepath;
	unsigned int _RendererID;	
	std::unordered_map<std::string, int> _UniformLocationCache; //Caching for uniforms
	
	std::string GraspShader(const std::string& Filepath)
	{
		std::ifstream stream(Filepath);
		std::string line;
		std::stringstream ss;

		while (getline(stream, line))
		{
			ss << line << '\n';
		}

		return ss.str();
	}

	unsigned int CompileShader(unsigned int Shadertype, const std::string& Source)
	{
		GLCall(unsigned int id = glCreateShader(Shadertype));
		const char* src = Source.c_str(); //Konvertierung in C-String
		GLCall(glShaderSource(id, 1, &src, nullptr));
		GLCall(glCompileShader(id));

		//Errorhandling Shadercompiling
		int result;
		GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
		if (result == GL_FALSE)
		{
			int length;
			GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
			char* message = (char*)alloca(length * sizeof(char));
			GLCall(glGetShaderInfoLog(id, length, &length, message));
			std::cout << "Failed to compile " << Shadertype << "shader!" << std::endl;
			std::cout << message << std::endl;
			GLCall(glDeleteShader(id));
			return 0;
		}

		return id;
	}

	unsigned int CreateShader(const std::string& vs_Source, const std::string& fs_Source)
	{
		GLCall(unsigned int program = glCreateProgram());
		GLCall(unsigned int c_vs = CompileShader(GL_VERTEX_SHADER, vs_Source));
		GLCall(unsigned int c_fs = CompileShader(GL_FRAGMENT_SHADER, fs_Source));

		GLCall(glAttachShader(program, c_vs));
		GLCall(glAttachShader(program, c_fs));
		GLCall(glLinkProgram(program));

		//Errorhandling Shaderlinking
		int success;
		char infoLog[512];
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(program, 512, NULL, infoLog);
			std::cout << "SHADER::LINKING::FAILED\n" << infoLog << std::endl;
		}

		GLCall(glValidateProgram(program));

		//----- technically
		GLCall(glDetachShader(program, c_vs));
		GLCall(glDetachShader(program, c_fs));

		GLCall(glDeleteShader(c_vs));
		GLCall(glDeleteShader(c_fs));

		return program;
	}

	int GetUniformLocation(const std::string& name)
	{
		if (_UniformLocationCache.find(name) != _UniformLocationCache.end())
			return _UniformLocationCache[name];

		GLCall(int location = glGetUniformLocation(_RendererID, name.c_str()));
		_UniformLocationCache[name] = location;

		return location;
	}

public:
	Shader(const std::string& vs_Filepath, const std::string& fs_Filepath)
		: _vs_Filepath(vs_Filepath), _fs_Filepath(fs_Filepath), _RendererID(0)
	{
		std::string vs_Source = GraspShader(_vs_Filepath);
		std::string fs_Source = GraspShader(_fs_Filepath);
		_RendererID = CreateShader(vs_Source, fs_Source);
	}

	~Shader()
	{
		GLCall(glDeleteProgram(_RendererID));
	}

	void bind() const
	{
		GLCall(glUseProgram(_RendererID));
	}

	void unbind() const
	{
		GLCall(glUseProgram(0));
	}	
};