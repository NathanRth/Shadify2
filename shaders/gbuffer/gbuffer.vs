#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 tangent;

out vec4 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec3 Tangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 viewPos = view * model * vec4(position, 1.0);
    FragPos = viewPos; 
    TexCoords = uv;
    
    mat3 normalMatrix = transpose(inverse(mat3(view * model)));
    Normal = normalMatrix * normal;
    Tangent = normalMatrix * tangent;

    gl_Position = projection * viewPos;
}