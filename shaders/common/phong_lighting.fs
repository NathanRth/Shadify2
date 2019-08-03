#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gSpecular;

struct Light {
    vec3 Position;
    vec3 Color;
    
    float Linear;
    float Quadratic;

    float Intensity;

    int Type;
    vec3 Direction;
};
uniform Light lights[4];
uniform int nbLights;

uniform mat4 view;

void main()
{             
    // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedo, TexCoords).rgb;
    float Specular = texture(gSpecular, TexCoords).r;
    
    vec3 lighting = vec3(0.0); 
    vec3 viewDir  = normalize(-FragPos); // viewpos is (0.0.0)
    for(int i = 0; i < nbLights; i++){
        
        // diffuse
        vec3 lightPos = (view * vec4(lights[i].Position,1.0)).xyz;
        vec3 lightDir = normalize(lightPos - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * lights[i].Color;
        
        // specular
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(Normal, halfwayDir), 0.0), 128.0)* Specular;
        vec3 specular = lights[i].Color * spec;
        
        // attenuation
        float distance = length(lightPos - FragPos);
        float attenuation = (1.0 / (1.0 + lights[i].Linear * distance + lights[i].Quadratic * distance * distance));
        diffuse *= attenuation;
        specular *= attenuation;
        
        lighting += diffuse+vec3(0.2)*Albedo + specular;
    }
    FragColor = vec4(lighting, 1.0);
}
