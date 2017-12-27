#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{             
    // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    
    // then calculate lighting as usual
    vec3 lighting  = Diffuse * 0.3; // hard-coded ambient component
    // diffuse
    vec3 lightDir = normalize(-lightDirection);
    vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse;
    // specular
	vec3 viewDir  = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
    vec3 specular = vec3(0.3) * spec * Specular;   
	
    FragColor = vec4( diffuse + lighting + specular, 1.0);
}