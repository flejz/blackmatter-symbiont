#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

#include "stb_image.h"

void loadImage(unsigned int &texture, std::string imageFilePath, GLint internalFormat, GLenum format) {
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, channelNumber;
  unsigned char *data = stbi_load(imageFilePath.c_str(), &width, &height, &channelNumber, 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Failed to load texture: " << imageFilePath << std::endl;
  }
  stbi_image_free(data);
}

void loadJPEG(unsigned int &texture, std::string imageFilePath) {
  loadImage(texture, imageFilePath, GL_RGB, GL_RGB);
}

void loadPNG(unsigned int &texture, std::string imageFilePath) {
  loadImage(texture, imageFilePath, GL_RGBA, GL_RGBA);
}
