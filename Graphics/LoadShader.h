#ifndef LOADSHADER_H_INCLUDED
#define LOADSHADER_H_INCLUDED


#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include <GL/glew.h>

//GLuint loadShader(const char* VertexShaderFileName, const char* FragmentShaderFileName);


class ShaderLoader
{
    private:
        enum ShaderType
        {
            ST_VERTEX_SHADER    = GL_VERTEX_SHADER,
            ST_GEOMETRY_SHADER  = GL_GEOMETRY_SHADER,
            ST_FRAGMENT_SHADER  = GL_FRAGMENT_SHADER

        };

        static std::string replaceConstatnsInLine(std::string line, const std::unordered_map<std::string, std::string>& constants);

        static bool loadShaderCode(const std::string& fileName, std::string& code, const std::vector<std::string>& defines,
                                   const std::unordered_map<std::string, std::string>& constants);
        static GLuint compileShader(ShaderType type, std::string& code);
        static GLuint linkProgram(GLuint vertexShaderId, GLuint fragmentShaderId);

    public:
        static GLuint loadShader(const std::string& vertexShaderFileName,
                                 const std::string& fragmentShaderFileName,
                                 std::vector<std::string> defines,
                                 std::unordered_map<std::string, std::string> constants);

};


#endif // LOADSHADER_H_INCLUDED
