#pragma once
#include <string>

class Texture {
public:
    Texture(const std::string& path);
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    void Bind(unsigned int unit = 0) const;

private:
    unsigned int ID = 0;
    int width = 0, height = 0, channels = 0;
};