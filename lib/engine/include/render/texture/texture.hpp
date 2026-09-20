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
    unsigned int m_ID = 0;
    int m_width = 0, m_height = 0, m_channels = 0;
};