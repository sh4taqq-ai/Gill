#pragma once
#include <string>

#include "mat4.hpp"

class Shader {
public:
    Shader(const std::string& vertPath,const std::string& fragPath);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    void Use() const;
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4f(const std::string& name, const mathpp::mat4f& matrix) const;
    void setVec2f(const std::string& name, const mathpp::vec2f& vector) const;
    void setVec3f(const std::string& name, const mathpp::vec3f& vector) const;

private:
    unsigned int ID;

};

