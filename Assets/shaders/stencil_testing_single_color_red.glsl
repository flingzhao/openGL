#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}
#type fragment
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(0.44, 0.04, 0.06, 1.0);
}

