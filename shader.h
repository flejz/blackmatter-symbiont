#pragma once
#include <string>

std::string parseShader(const std::string &filepath);
unsigned int compileShader(unsigned int type, const std::string &source);

void setBool(unsigned int program, const std::string &name, bool value);
void setInt(unsigned int program, const std::string &name, int value);
void setFloat(unsigned int program, const std::string &name, float value);

