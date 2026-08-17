#pragma once
#include <string>

#include "mat4.hpp"

class Shader {
public:
    Shader(const std::string& vertPath,const std::string& fragPath);
    void Use();
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4f(const std::string& name, const mathpp::mat4f& matrix) const;
private:
    unsigned int ID;

};

