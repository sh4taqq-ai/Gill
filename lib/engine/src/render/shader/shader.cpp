#include "../include/render/shader/shader.hpp"
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <mathpp.hpp>
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
        if (!vertexShaderCode.is_open()) {
            std::cerr << "Failed to open vertex shader file: " << vertPath << std::endl;
        }
        std::ifstream fragmentShadeCode(fragPath);
        if (!fragmentShadeCode.is_open()) {
            std::cerr << "Failed to open fragment shader file: " << fragPath << std::endl;
        }
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
    int successv;
    char infoLogv[512];

    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successv);
    if (!successv) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLogv);
        std::cerr << "Vertex shader compile error:\n" << infoLogv << std::endl;
    }
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragCode, NULL);
    int successf;
    char infoLogf[512];

    glCompileShader(fragmentShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successf);
    if (!successf) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLogf);
        std::cerr << "Fragment shader compile error:\n" << infoLogf << std::endl;
    }



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

void Shader::setMat4f(const std::string &name, const mathpp::mat4f& matrix ) const {
    const float* p = &matrix.col[0][0];
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()),1,GL_FALSE,p);
}

void Shader::Use() {
    glUseProgram(ID);
}
