#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

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

// uniform sampler2D texture_diffuse1;
uniform vec3 lightColor;

void main()
{    
    // FragColor = vec4(1.0, 0.2, 0.3, 1.0);
    // FragColor = texture(texture_diffuse1, TexCoords);
    FragColor = vec4(lightColor, 0.2);
}