#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "stb_image.h"
#include "texture.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void processingInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

float vertices[] = {
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
  0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
  -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

  -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
  -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL) 
  {
    std::cout << "Failed to create GLFW windows" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  // initialize glew
  if (glewInit() != GLEW_OK)
  {
    std::cout << "Glew error!" << std::endl;
    return -1;
  }

  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  glEnable(GL_DEPTH_TEST);

  int success;
  char infoLog[512];

  // vertex shader
  const std::string vertexShaderSource = parseShader("vertex.glsl");
  unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);

  // fragment shader
  const std::string fragmentShaderSource = parseShader("fragment.glsl");
  unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

  // shader program
  unsigned int program;
  program = glCreateProgram();

  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
  }
  
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // indices for element buffer
  unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
  };  

  // >>>>>>>>> VERTICE ARRAY <<<<<<<<<
  // vertex array object
  unsigned int vao;
  glGenVertexArrays(1, &vao);

  // vertex buffer object
  unsigned int vbo;
  glGenBuffers(1, &vbo);

  // generate and assign data array and buffer, as well as element buffer
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // >>>>>>>>> TEXTURE <<<<<<<<<
  // texture creation
  unsigned int texture1, texture2;

  stbi_set_flip_vertically_on_load(true);
  loadJPEG(texture1, "container.jpg");
  loadPNG(texture2, "bender.png");

  glUseProgram(program);
  setInt(program, "uniTexture1", 0);
  setInt(program, "uniTexture2", 1);

  // >>>>>>>>> 3D <<<<<<<<<
  // transformations
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection;

  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
  projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

  unsigned int viewPos = glGetUniformLocation(program, "view");
  unsigned int projectionPos = glGetUniformLocation(program, "projection");

  glUniformMatrix4fv(viewPos, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(projectionPos, 1, GL_FALSE, glm::value_ptr(projection));

  while (!glfwWindowShouldClose(window))
  {
    // input
    processingInput(window);

    // rendering commands here
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // setting respective textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // drawing shaders
    glUseProgram(program);

    // transformation - rotate
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));  
    unsigned int modelPos = glGetUniformLocation(program, "model");
    glUniformMatrix4fv(modelPos, 1, GL_FALSE, glm::value_ptr(model));

    // render container
    glBindVertexArray(vao);

    // drawing vectors
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // check and call events and swap the buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);

  glfwTerminate();
  return 0;
}
