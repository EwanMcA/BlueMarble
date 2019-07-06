#include "bmpch.h"
#include "Shader.h"

#include <glad/glad.h>

namespace BlueMarble {

    Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        // Create an empty vertex shader handle
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

        // Send the vertex shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        const GLchar* source = vertexSrc.c_str();
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
        source = fragmentSrc.c_str();
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
        oRendererId = glCreateProgram();

        // Attach our shaders to our program
        glAttachShader(oRendererId, vertexShader);
        glAttachShader(oRendererId, fragmentShader);

        // Link our program
        glLinkProgram(oRendererId);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(oRendererId, GL_LINK_STATUS, (int *)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(oRendererId, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(oRendererId, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(oRendererId);
            // Don't leak shaders either.
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            BM_CORE_ERROR("{0}", infoLog.data());
            BM_CORE_ASSERT(false, "Shader linking failure!");
            return;
        }

        // Always detach shaders after a successful link.
        glDetachShader(oRendererId, vertexShader);
        glDetachShader(oRendererId, fragmentShader);
    }

    Shader::~Shader()
    {
        glDeleteProgram(oRendererId);
    }

    void Shader::Bind() const
    {
        glUseProgram(oRendererId);
    }

    void Shader::Unbind() const
    {
        glUseProgram(0);
    }

    //From OPENGL Series
    /*ShaderSource Shader::ParseShader(const std::string& filepath)
    {
        std::ifstream stream(filepath);

        enum class ShaderType
        {
            NONE = -1, VERTEX = 0, FRAGMENT = 1
        };

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
    }*/

} // namespace BlueMarble