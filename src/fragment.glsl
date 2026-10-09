#version 330 core

in vec3 vertex2fragment_color;
out vec4 FragColor;

uniform vec4 redValue;

void main()
{
   FragColor = vec4(
      redValue.x,
      vertex2fragment_color.y,
      vertex2fragment_color.z,
      1.f);
}