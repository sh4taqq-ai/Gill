#pragma once
#include <string>

class Shader {
public:
    Shader(const std::string& vertPath,const std::string& fragPath);
    ~Shader();
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
private:
    unsigned int ID;

};

