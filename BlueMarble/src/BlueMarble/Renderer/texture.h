#pragma once

namespace BlueMarble {

    class Texture
    {
    public:
        virtual ~Texture() {}

        virtual void Bind(unsigned int slot = 0) const = 0;
        virtual void Unbind() const = 0;
        
        static Texture* Texture::Create(const std::string& path);
    };

} // namespace BlueMarble