#include "bmpch.h"
#include "openGLShader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace BlueMarble {

    OpenGLShader::OpenGLShader(const OpenGLShader::ShaderSource& src)
    {
        // Create an empty vertex shader handle
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

        // Send the vertex shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        const GLchar* source = src.vertexSrc.c_str();
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

            BM_CORE_ERROR("{0}", infoLog.data());
            BM_CORE_ASSERT(false, "Vertex Shader compilation failure!");
            return;
        }

        // Create an empty fragment shader handle
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        // Send the fragment shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        source = src.fragmentSrc.c_str();
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

            BM_CORE_ERROR("{0}", infoLog.data());
            BM_CORE_ASSERT(false, "Fragment Shader compilation failure!");
            return;
        }

        // Vertex and fragment shaders are successfully compiled.
        // Now time to link them together into a program.
        // Get a program object.
        oRendererID = glCreateProgram();

        // Attach our shaders to our program
        glAttachShader(oRendererID, vertexShader);
        glAttachShader(oRendererID, fragmentShader);

        // Link our program
        glLinkProgram(oRendererID);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(oRendererID, GL_LINK_STATUS, (int *)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(oRendererID, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(oRendererID, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(oRendererID);
            // Don't leak shaders either.
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            BM_CORE_ERROR("{0}", infoLog.data());
            BM_CORE_ASSERT(false, "Shader linking failure!");
            return;
        }

        // Always detach shaders after a successful link.
        glDetachShader(oRendererID, vertexShader);
        glDetachShader(oRendererID, fragmentShader);
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(oRendererID);
    }

    void OpenGLShader::Bind() const
    {
        glUseProgram(oRendererID);
    }

    void OpenGLShader::Unbind() const
    {
        glUseProgram(0);
    }

    void OpenGLShader::UploadUniformInt(const std::string& name, const int value)
    {
        GLint location = GetUniformLocation(name);
        glUniform1i(location, value);
    }

    void OpenGLShader::UploadUniformFloat(const std::string & name, const float value)
    {
        GLint location = GetUniformLocation(name.c_str());
        glUniform1f(location, value);
    }

    void OpenGLShader::UploadUniformFloat2(const std::string & name, const glm::vec2 & values)
    {
        GLint location = GetUniformLocation(name.c_str());
        glUniform2f(location, values.x, values.y);
    }

    void OpenGLShader::UploadUniformFloat3(const std::string & name, const glm::vec3 & values)
    {
        GLint location = GetUniformLocation(name.c_str());
        glUniform3f(location, values.x, values.y, values.z);
    }
    void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
    {
        GLint location = GetUniformLocation(name);
        glUniform4f(location, values.x, values.y, values.z, values.w);
    }

    void OpenGLShader::UploadUniformMat3(const std::string & name, const glm::mat3 & matrix)
    {
        GLint location = GetUniformLocation(name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
    {
        GLint location = GetUniformLocation(name);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    // From OPENGL Series
    OpenGLShader::ShaderSource OpenGLShader::ParseShader(const std::string& filepath)
    {
        std::ifstream stream(filepath);

        std::string line;
        std::stringstream ss[2];
        ShaderType type = ShaderType::NONE;
        while (getline(stream, line))
        {
            if (line.find("#shader") != std::string::npos) {

                if (line.find("vertex") != std::string::npos)
                    type = ShaderType::VERTEX;
                else if (line.find("fragment") != std::string::npos)
                    type = ShaderType::FRAGMENT;
            }
            else {
                ss[(int)type] << line << std::endl;
            }
        }

        return { ss[0].str(), ss[1].str() };
    }

    uint32_t OpenGLShader::GetUniformLocation(const std::string & name) const
    {
        if (oUniformLocationCache.find(name) != oUniformLocationCache.end())
            return oUniformLocationCache[name];

        GLint location = glGetUniformLocation(oRendererID, name.c_str());
        oUniformLocationCache.insert({ name, location });
        return location;
    }

} // namespace BlueMarble