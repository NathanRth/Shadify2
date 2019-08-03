#version 330 core
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out float gDepth;
layout (location = 4) out float gSpecular;

in vec2 TexCoords;
in vec4 FragPos;
in vec3 Normal;
in vec3 Tangent;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

uniform vec3 meshAlbedo;

uniform bool hasDiffuse;
uniform bool hasSpecular;
uniform bool hasNormal;
uniform bool hasHeight;

const float FAR = 100.0;
const float NEAR = 0.1;

float linearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));
}

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    gPosition.rgb = FragPos.xyz;
    float ld = linearizeDepth(gl_FragCoord.z);
    gPosition.a = ld;
    gDepth = ld;

    if(hasNormal){
        vec3 bitangent = cross(Normal, Tangent);
        vec3 mapNormal = texture(texture_normal1, TexCoords).xyz;
        mapNormal = 2.0 * mapNormal - vec3(1.0);
        mat3 TBN = mat3(Tangent, bitangent, Normal);
        gNormal = normalize(TBN * mapNormal);
    }
    
    else{
        gNormal = normalize(Normal);
    }

    // and the diffuse per-fragment color
    if(hasDiffuse){
        gAlbedo = (texture(texture_diffuse1, TexCoords).rgb);
    }

    else{
        gAlbedo = meshAlbedo.rgb;
    }

    if(hasSpecular){
        gSpecular = (texture(texture_specular1, TexCoords).r);
    }
    
    else{
        gSpecular = (0.1);
    }
    
}