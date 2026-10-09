#version 330 core
in vec3 vertex2fragment_color;
out vec4 FragColor;
void main()
{
   FragColor = vec4(vertex2fragment_color.x, vertex2fragment_color.y, vertex2fragment_color.z, 1.0f);
}