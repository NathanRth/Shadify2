#version 330 core

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform sampler2D final;
uniform sampler2D gDepth;

uniform bool useSkybox;

void main()
{    
    vec3 accLight = texture(final, gl_FragCoord.xy / vec2(1280, 720)).rgb;
    float depth = texture(gDepth, gl_FragCoord.xy / vec2(1280, 720)).r;
    if(depth <= 0.2 && useSkybox)
    {
        if(useSkybox)
        {
            FragColor = texture(skybox, TexCoords);
        }
        else
        {
            FragColor = vec4(accLight,1.0);
        }
    }
    else
    {
        FragColor = vec4(accLight,1.0);
    }
    //FragColor = vec4(accLight,1.0);
    //FragColor = mix(texture(skybox, TexCoords), vec4(accLight,1.0),  depth);
}
