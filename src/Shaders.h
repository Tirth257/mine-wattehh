#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    unsigned int ID;

    // Existing constructor (Opaque blocks)
    Shader(const char* vertexPath, const char* fragmentPath) {
        std::string vCode = loadFile(vertexPath);
        std::string fCode = loadFile(fragmentPath);
        
        unsigned int sV = compileShader(GL_VERTEX_SHADER, vCode.c_str(), "VERTEX");
        unsigned int sF = compileShader(GL_FRAGMENT_SHADER, fCode.c_str(), "FRAGMENT");

        ID = glCreateProgram();
        glAttachShader(ID, sV);
        glAttachShader(ID, sF);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        glDeleteShader(sV);
        glDeleteShader(sF);
    }

    // NEW Constructor for Tessellation (Water)
    Shader(const char* vertexPath, const char* fragmentPath, const char* tcsPath, const char* tesPath) {
        std::string vCode = loadFile(vertexPath);
        std::string fCode = loadFile(fragmentPath);
        std::string tcsCode = loadFile(tcsPath);
        std::string tesCode = loadFile(tesPath);

        unsigned int sV = compileShader(GL_VERTEX_SHADER, vCode.c_str(), "VERTEX");
        unsigned int sF = compileShader(GL_FRAGMENT_SHADER, fCode.c_str(), "FRAGMENT");
        unsigned int sTCS = compileShader(GL_TESS_CONTROL_SHADER, tcsCode.c_str(), "TCS");
        unsigned int sTES = compileShader(GL_TESS_EVALUATION_SHADER, tesCode.c_str(), "TES");

        ID = glCreateProgram();
        glAttachShader(ID, sV);
        glAttachShader(ID, sTCS);
        glAttachShader(ID, sTES);
        glAttachShader(ID, sF);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        glDeleteShader(sV);
        glDeleteShader(sTCS);
        glDeleteShader(sTES);
        glDeleteShader(sF);
    }

    void use() { glUseProgram(ID); }

private:
    // Helper to read file to string
    std::string loadFile(const char* path) {
        std::string code;
        std::ifstream file;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            file.open(path);
            std::stringstream stream;
            stream << file.rdbuf();
            file.close();
            code = stream.str();
        } catch (std::ifstream::failure& e) {
            std::cout << "ERROR::SHADER::FILE_NOT_READ: " << path << " | " << e.what() << std::endl;
        }
        return code;
    }

    // Helper to compile a single stage
    unsigned int compileShader(GLenum type, const char* source, std::string name) {
        unsigned int s = glCreateShader(type);
        glShaderSource(s, 1, &source, NULL);
        glCompileShader(s);
        checkCompileErrors(s, name);
        return s;
    }

    void checkCompileErrors(unsigned int shader, std::string type) {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog << std::endl;
            }
        }
    }
};
#endif