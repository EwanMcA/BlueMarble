#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

namespace BlueMarble {

    class Shader
    {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        
        static Shader* Create(const std::string& filename);
    };

} // namespace BlueMarble
