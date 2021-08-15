#version 330 core
in vec4 vertexColor; // same name of an output from the fragment shader, just gets passed without further iteration
out vec4 fragColor; 
uniform vec4 ourColor;

void main()
{
  fragColor = ourColor;
}
