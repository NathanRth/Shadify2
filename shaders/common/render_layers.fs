#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPositionDepth;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gDepth;
uniform sampler2D gSpecular;
uniform sampler2D texSSDO;
uniform sampler2D texSSAO;
uniform sampler2D texLighting;

uniform int draw_mode;

void main()
{
    vec4 fragPos = texture(gPositionDepth, TexCoords);
    vec3 normal = texture(gNormal, TexCoords).rgb;	
    vec3 albedo = texture(gAlbedo, TexCoords).rgb;
    float depth = texture(gDepth, TexCoords).r;
    float spec = texture(gSpecular, TexCoords).r;
    vec3 lighting = texture(texLighting, TexCoords).rgb;
    vec3 ssdo = texture(texSSDO, TexCoords).rgb;
    float ssao = texture(texSSAO, TexCoords).r;
        
    // Based on which of the 1 - 9, 0 keys we pressed, show specific buffer values
    if(draw_mode == 1)//albedo
        FragColor = vec4(albedo,1.0);
    else if(draw_mode == 2)//lighting
        FragColor = vec4(lighting,1.0);
    else if(draw_mode == 3)// ssao
        FragColor = vec4(vec3(ssao),1.0);
    else if(draw_mode == 4)// ssao + lighting
        FragColor = vec4(lighting*ssao,1.0);
    else if(draw_mode == 5)// ssdo
        FragColor = vec4(ssdo,1.0);
    else if(draw_mode == 6)// ssdo + lighting
        FragColor = vec4(lighting*ssdo,1.0);
    else if(draw_mode == 7)
        FragColor = vec4(vec3(fragPos.a/50.0),1.0);    
}