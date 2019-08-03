#version 330 core

#define M_PI 3.14159265
#define MAX_NB_LIGHTS   4
// ssdo outputs colored occlusion

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D texLighting;
uniform sampler2D texNoise;
uniform samplerCube skybox;

uniform bool useSkybox;

uniform int mode;

// number of samples
const int kernelSize = 16;
uniform vec3 samples[kernelSize];

struct Light{
    vec3 Position;
    vec3 Color;
    int Type; // Point | Directionnal
    vec3 Direction;
};

uniform int nbLights;
uniform Light lights[MAX_NB_LIGHTS];

uniform mat4 projection;
uniform mat4 view;
uniform mat4 iview;//inverse view matrix

// radius of the hemisphere around fragment
uniform float radius;
uniform float depthClamp = 0.02;


const vec2 noiseScale = vec2(1280.0/4.0, 720.0/4.0); 

void main()
{
    vec3 fragPos = texture(gPosition, TexCoords).xyz;
    vec3 fragAlbedo = texture(gAlbedo, TexCoords).rgb;
    vec3 fragNormal = normalize(texture(gNormal, TexCoords).xyz);
    vec3 randomVec = normalize(texture(texNoise, TexCoords * noiseScale).xyz);

    vec3 directLight = vec3(0.0);
    vec3 indirectLight = vec3(0.0);

    // create TBN change-of-basis matrix: from tangent-space to view-space
    vec3 tangent = normalize(randomVec - fragNormal * dot(randomVec, fragNormal));
    vec3 bitangent = cross(fragNormal, tangent);
    mat3 TBN = mat3(tangent, bitangent, fragNormal);

    float As = (M_PI*radius*radius)/kernelSize;

    // for each sample
    for(int i = 0; i < kernelSize; ++i)
    {
        // get sample position
        vec3 samplePos = fragPos + ((TBN * samples[i]) * radius);

        // project sample position
        vec4 offset = vec4(samplePos, 1.0);
        offset = projection * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;

        //get sample depth
        vec3 sampleProjPos = texture(gPosition, offset.xy).xyz;
        float sampleDepth = sampleProjPos.z;

        vec3 sampleColor = vec3(0.0);

        vec3 wi = normalize(samplePos - fragPos);
        float theta = dot(fragNormal,wi);

        // ####################### DIRECT PASS #######################################

        if(mode == 0 || mode == 2)
        {
            if(sampleDepth < samplePos.z+0.025)
            {
                // for each light in scene
                for(int j = 0;j<nbLights;j++)
                {
                    vec3 lightPos = (view * vec4(lights[j].Position,1.0)).xyz;
                    vec3 lj = lightPos - fragPos;
                    
                    float phi = dot(wi, normalize(lj));            
                    if(phi > 0.0)
                    {
                        float d = distance(fragPos, lightPos);
                        // la puissance spécifique d'une source lumineuse (ici le 2.0) peut être
                        // contrôlé indépendement si on ajoute un champ à la structure de la lumière
                        sampleColor += pow(phi,(d*d)/2.0) * lights[j].Color;
                    }
                }
                
                // if we use a skybox for illumination
                if(useSkybox)
                {
                    vec4 skyboxDirection = iview * vec4(wi, 0.0);
			        vec3 skyboxColor = texture(skybox, skyboxDirection.xyz).xyz;
			        sampleColor += skyboxColor;
                }
                else
                {
                    sampleColor += vec3(1.0);
                }
                directLight += sampleColor;
            }
        }// direct light

        // ####################### INDIRECT PASS #######################################

        if(mode == 1 || mode == 2)
        {
            //if the projected depth of the sample lands bellow
            // the hemisphere base, it is not counted for indirect pass

            if(sampleDepth <= samplePos.z)
            {
                // individual test to ease computation
                float tri = dot(fragNormal,sampleProjPos-fragPos);
                if(tri >= 0.0)
                {
                    vec3 sampleNormal = normalize(texture(gNormal, offset.xy).xyz);
                    vec3 sampToFrag = normalize(fragPos - sampleProjPos);
                    float tsi = dot(sampleNormal, sampToFrag);

                    if(tsi >= 0.0)
                    {
                        vec3 color = texture(texLighting, offset.xy).rgb + sampleColor;
                        float d = min(distance(sampleProjPos, fragPos),1.0);
                        
                        indirectLight += clamp(As*((tri*tsi)/(d*d)),0.0,1.0)*color;
                    }
                }
            }

        }//indirect path

    }// end for each sample

    directLight /= kernelSize;
    indirectLight /= kernelSize;

    switch(mode){
    case 0:
        FragColor = vec4(directLight, 1.0);
        break;
    case 1:
        FragColor = vec4(indirectLight, 1.0);
        break;
    case 2:
        FragColor = vec4(directLight + indirectLight, 1.0);
        break;
    }
    
    
}
