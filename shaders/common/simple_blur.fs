#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D samplerInput;

void main() 
{
    
    vec2 texelSize = 1.0 / vec2(textureSize(samplerInput, 0));
    vec3 result = vec3(0);
    int radius = 2;
    for (int x = -radius; x < radius; ++x) 
    {
        for (int y = -radius; y < radius; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(samplerInput, TexCoords + offset).rgb;
        }
    }
    FragColor = vec4(result / (radius*2 * radius*2),1.0);
    //FragColor = texture(samplerInput, TexCoords);
}  
