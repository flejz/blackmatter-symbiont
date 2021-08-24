#version 330 core
in vec3 fragColor;
in vec2 fragTexCoords;

out vec4 outColor; 

uniform sampler2D uniTexture1;
uniform sampler2D uniTexture2;

void main()
{
  outColor = mix(texture(uniTexture1, fragTexCoords), texture(uniTexture2, fragTexCoords), 0.4); 
}
