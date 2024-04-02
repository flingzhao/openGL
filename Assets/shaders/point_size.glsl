
#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    gl_PointSize = gl_Position.z;
}

#type fragment
#version 330 core
out vec4 FragColor;

void main()
{    
    FragColor = vec4(1.0f, 0.2f, 0.3f, 1.0f);
}