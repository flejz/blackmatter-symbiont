#version 330 core
in vec3 fragColor;
in vec2 fragTexCoords;

out vec4 outColor; 

uniform sampler2D uniTexture;

void main()
{
  // outColor = vec4(fragColor, 1.0);
  outColor = texture(uniTexture, fragTexCoords) * vec4(fragColor, 1.0);
}
