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
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#type fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;
uniform sampler2D zk;

void main()
{   
    if(gl_FrontFacing)
        FragColor = texture(texture1, TexCoords);
    else
        FragColor = texture(zk, TexCoords);
        // FragColor = vec4(1.0f, 0.3f, 0.2f, 1.0f);
}