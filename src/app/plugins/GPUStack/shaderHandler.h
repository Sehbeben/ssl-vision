#include <iostream>
#include <vector>
#include <fstream>

#include <GL/glew.h>


class ShaderHandler{

public:

  
  static GLuint createShaderProgram(const char* path2Vertex, const char* path2fragment);
  
private:  
  
  enum ShaderType
  {
    FRAGMENT,
    VERTEX
  };

  
  static std::string ReadShaderFile(const char* file_path);
  static GLuint compileShader(ShaderType type, std::string shader_source);
  static GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);
  //static GLuint LoadShader(ShaderType type, const char* file_path);
 // static GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);
  
};