#include "shader.hpp"
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <sstream>
Shader::Shader(const std::string& vertPath,const std::string& fragPath) {
    std::stringstream vertStream;
    std::stringstream fragStream;
    std::string vertStr;
    std::string fragStr;

    try {
        std::ifstream vertexShaderCode(vertPath);
        std::ifstream fragmentShadeCode(fragPath);
        vertStream << vertexShaderCode.rdbuf();
        fragStream << fragmentShadeCode.rdbuf();
        vertStr = vertStream.str();
        fragStr = fragStream.str();
    }
    catch (std::ifstream::failure& e) {
        std::cerr << e.what() << std::endl;
    }

    const char* vertCode = vertStr.c_str();
    const char* fragCode = fragStr.c_str();

    unsigned int vertexShader, fragmentShader;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertCode, NULL);
    glCompileShader(vertexShader);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragCode, NULL);
    glCompileShader(fragmentShader);

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    glValidateProgram(ID);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()),static_cast<int> (value));
}
void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()),value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()),value);
}

