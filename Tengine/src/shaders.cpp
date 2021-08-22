#include "tpch.h"
#include "shader/shaderall.h"
#include "util/utillog.h"
#include "glm/gtc/type_ptr.hpp"
#include "glfw/glfw3.h"
#include "glad/glad.h"

namespace tengine
{
	//shader

	shader* shader::create(const std::string& vsrc, const std::string& fsrc)
	{

		//switch (renderer::getAPI())
		//{
		//case API::None: log("giving unsupported renderer API, exit...", 3); exit(EXIT_FAILURE);
		//case API::opengl: return new openglShader(vsrc, fsrc);
		//default: log("directx and vulkan is not supported yet, exit...", 3); exit(EXIT_FAILURE);
		//}
		return NULL; // no longer used
	}
	shader* shader::create(const std::string& src)
	{

		//switch (renderer::getAPI())
		//{
		//case API::None: log("giving unsupported renderer API, exit...", 3); exit(EXIT_FAILURE);
		//case API::opengl: return new openglShader(src);
		//default: log("directx and vulkan is not supported yet, exit...", 3); exit(EXIT_FAILURE);
		//}
		return NULL; // no longer used
	}

	// opengl Shader

	openglShader::openglShader(const std::string& vsrc, const std::string& fsrc)
	{
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = (const GLchar*)vsrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			log("vertex shader compilation failed", 3);
			exit(EXIT_FAILURE);
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar*)fsrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			log("fragment shader compilation failed", 3);
			exit(EXIT_FAILURE);
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		renID = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(renID, vertexShader);
		glAttachShader(renID, fragmentShader);

		// Link our program
		glLinkProgram(renID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(renID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(renID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(renID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(renID);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(renID, vertexShader);
		glDetachShader(renID, fragmentShader);
	}
	openglShader::openglShader(const std::string& src)
	{
		std::string secString;
		std::ifstream in(src, std::ios::in, std::ios::binary);
		if (!in) errout("shader file fail to load");
		in.seekg(0, std::ios::end);
		secString.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&secString[0], secString.size());
		in.close();

		std::unordered_map<GLenum, std::string> shaMap;
		const char* token = "#type";
		size_t pos = secString.find(token, 0);
		while (pos != std::string::npos)
		{
			size_t eol = secString.find_first_of("\r\n", pos);
			size_t begin = pos + strlen(token) + 1;
			std::string type = secString.substr(begin, eol - begin);
			GLenum shaType;
			if (type == "vertex")
				shaType = GL_VERTEX_SHADER;
			else if (type == "fragment")
				shaType = GL_FRAGMENT_SHADER;
			else
				errout("invalid shader type given");
			size_t nextLinePos = secString.find_first_not_of("\r\n", eol);
			pos = secString.find(token, nextLinePos);
			shaMap[shaType] =
				secString.substr(nextLinePos,
					pos - (nextLinePos == std::string::npos ? secString.size() - 1 : nextLinePos));
		}

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		std::string vsrc = shaMap[GL_VERTEX_SHADER];
		std::string fsrc = shaMap[GL_FRAGMENT_SHADER];

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = (const GLchar*)vsrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			log("vertex shader compilation failed", 3);
			exit(EXIT_FAILURE);
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = (const GLchar*)fsrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			log("fragment shader compilation failed", 3);
			exit(EXIT_FAILURE);
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		renID = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(renID, vertexShader);
		glAttachShader(renID, fragmentShader);

		// Link our program
		glLinkProgram(renID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(renID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(renID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(renID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(renID);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(renID, vertexShader);
		glDetachShader(renID, fragmentShader);
	}

	openglShader::~openglShader()
	{
		glDeleteProgram(renID);
	}

	void openglShader::bind() const
	{
		glUseProgram(renID);
	}

	void openglShader::unbind() const
	{
		glUseProgram(0);
	}

	void openglShader::setUnimat4(const std::string& n, const glm::mat4& val) const
	{
		if (glGetUniformLocation(renID, n.c_str()) < 0)
		{
			log("value not found in this renderer", 2);
			std::cout << "value name: " << n << std::endl;
		}
		glUniformMatrix4fv(glGetUniformLocation(renID, n.c_str()), 1, GL_FALSE, glm::value_ptr(val));
	}
	void openglShader::setUnimat3(const std::string& n, const glm::mat3& val) const
	{
		if (glGetUniformLocation(renID, n.c_str()) < 0)
		{
			log("value not found in this renderer", 2);
			std::cout << "value name: " << n << std::endl;
		}
		glUniformMatrix3fv(glGetUniformLocation(renID, n.c_str()), 1, GL_FALSE, glm::value_ptr(val));
	}
	void openglShader::setUnivec4(const std::string& n, const glm::vec4& val) const
	{
		if (glGetUniformLocation(renID, n.c_str()) < 0)
		{
			log("value not found in this renderer", 2);
			std::cout << "value name: " << n << std::endl;
		}
		glUniform4f(glGetUniformLocation(renID, n.c_str()), val.x, val.y, val.z, val.w);
	}
	void openglShader::setUnivec3(const std::string& n, const glm::vec3& val) const
	{
		if (glGetUniformLocation(renID, n.c_str()) < 0)
		{
			log("value not found in this renderer", 2);
			std::cout << "value name: " << n << std::endl;
		}
		glUniform3f(glGetUniformLocation(renID, n.c_str()), val.x, val.y, val.z);
	}
	void openglShader::setUnivec2(const std::string& n, const glm::vec2& val) const
	{
		if (glGetUniformLocation(renID, n.c_str()) < 0)
		{
			log("value not found in this renderer", 2);
			std::cout << "value name: " << n << std::endl;
		}
		glUniform2f(glGetUniformLocation(renID, n.c_str()), val.x, val.y);
	}
	void openglShader::setUniint(const std::string& n, int val) const
	{
		if (glGetUniformLocation(renID, n.c_str()) < 0)
		{
			log("value not found in this renderer", 2);
			std::cout << "value name: " << n << std::endl;
		}
		glUniform1i(glGetUniformLocation(renID, n.c_str()), val);
	}
	void openglShader::setUnifloat(const std::string& n, float val) const
	{
		if (glGetUniformLocation(renID, n.c_str()) < 0)
		{
			log("value not found in this renderer", 2);
			std::cout << "value name: " << n << std::endl;
		}
		glUniform1f(glGetUniformLocation(renID, n.c_str()), val);
	}
	void openglShader::setUniintArray(const std::string& n, int* val, uint32_t count) const
	{
		if (glGetUniformLocation(renID, n.c_str()) < 0)
		{
			log("value not found in this renderer", 2);
			std::cout << "value name: " << n << std::endl;
		}
		glUniform1iv(glGetUniformLocation(renID, n.c_str()), count, val);
	}
	void openglShader::setUnifloatArray(const std::string& n, float* val, uint32_t count) const
	{
		if (glGetUniformLocation(renID, n.c_str()) < 0)
		{
			log("value not found in this renderer", 2);
			std::cout << "value name: " << n << std::endl;
		}
		glUniform1fv(glGetUniformLocation(renID, n.c_str()), count, val);
	}

	// shader lib
	void shaderLib::add(const std::string& n, const std::shared_ptr<shader>& sha)
	{ lib[n] = sha; }
}