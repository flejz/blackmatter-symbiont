#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

std::string parseShader(const std::string &filepath) {
  std::ifstream stream(filepath);
  std::string line;
  std::stringstream ss;

  while (getline(stream, line)) {
      ss << line << "\n";
  }

  return ss.str(); // take the first memory address of the string,
}

unsigned int compileShader(unsigned int type, const std::string &source) {
  unsigned int id = glCreateShader(type);
  const char *src = &source[0]; // take the first memory address of the string,
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char *message = (char *)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);

    std::cout << "Failed to compile "
         << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!"
         << std::endl;
    std::cout << message << std::endl;
    glDeleteShader(id);
    return 0;
  }

  return id;
}

void setBool(unsigned int program, const std::string &name, bool value)
{         
      glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value); 
}

void setInt(unsigned int program, const std::string &name, int value)
{ 
      glUniform1i(glGetUniformLocation(program, name.c_str()), value); 
}

void setFloat(unsigned int program, const std::string &name, float value)
{ 
      glUniform1f(glGetUniformLocation(program, name.c_str()), value); 
} 

