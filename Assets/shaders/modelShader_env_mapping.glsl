#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Position;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;    
    Position = vec3(model * vec4(aPos, 1.0));
    Normal = aNormal;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#type fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Position;
in vec3 Normal;

uniform samplerCube skybox;
uniform sampler2D texture_diffuse1;
uniform sampler2D cube;
uniform vec3 cameraPos;

void main()
{    
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));

    // // refract
    // float ratio = 1.00 / 1.52;
    // vec3 I = normalize(Position - cameraPos);
    // vec3 R = refract(I, normalize(Normal), ratio);
    // FragColor = texture(cube, TexCoords);
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
    FragColor = mix(FragColor, texture(texture_diffuse1, TexCoords), 0.6f);
    // FragColor = mix(texture(texture_diffuse1, TexCoords), texture(skybox, R), 0.4f);
    // vec3 res = texture(skybox, R).rgb * vec3(texture(texture_diffuse1, TexCoords));
    // FragColor = vec4(res, 1.0f);
}