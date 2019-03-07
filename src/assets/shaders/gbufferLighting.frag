#version 430 core
layout (location = 0) out vec4 FragColor;

layout (location = 0) in vec2 TexCoords;

layout(binding = 0) uniform sampler2D gDepth;
layout(binding = 1) uniform sampler2D gAlbedoSpec;
layout(binding = 2) uniform sampler2D gNormal;
layout(binding = 3) uniform sampler2D shadowMap;
layout(binding = 4) uniform sampler2D ssao;

layout (location = 0) uniform mat4 InverseProjectionMatrix;
layout (location = 1) uniform mat4 lightSpaceMatrix;
layout (location = 2) uniform vec3 viewPos;
layout (location = 3) uniform int type;
layout (location = 4) uniform vec3 Lightdirection;
layout (location = 5) uniform vec3 Lightambient;
layout (location = 6) uniform vec3 Lightdiffuse;
layout (location = 7) uniform vec3 Lightspecular;
struct DirectionalLight {
	vec3 direction;

	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
//uniform DirectionalLight light;

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

vec3 getPos()
{
	vec4 p = InverseProjectionMatrix * vec4(TexCoords * 2.0 - 1.0, texture(gDepth, TexCoords).r* 2.0 - 1.0, 1);
	vec3 viewspace_position = p.xyz / p.w;

	return viewspace_position;
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 Normal, vec3 lightDir)
{
	//SHADOW
	// perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;
	// get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	// get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;
	// calculate bias (based on depth map resolution and slope)
	vec3 normal = normalize(Normal);
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	// check whether current frag pos is in shadow
	// float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
	// PCF
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;

	// keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
	if (projCoords.z > 1.0)
		shadow = 0.0;

	return shadow;
}

void main()
{             
    // retrieve data from gbuffer
    vec3 FragPos = getPos();
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    //float AmbientOcclusion = texture(ssao, TexCoords).r;
	float AmbientOcclusion = 1.0f;
	vec3 ambient = Lightambient * Diffuse * AmbientOcclusion;
    // diffuse
    vec3 lightDir = normalize(-Lightdirection);
    float diff = max(dot(Normal, lightDir), 0.0);
	vec3 diffuse = Lightdiffuse * diff * Diffuse;
    // specular
	vec3 viewDir  = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
    vec3 specular = Lightspecular * spec * Specular;
	float shadow = 0.0f;
	if (type == 1) {
		vec4 FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
		shadow = ShadowCalculation(FragPosLightSpace, Normal, lightDir);
	}
	//vec4 fogColor = vec4(0.0118, 0.0510, 0.1215);
	//float fogStart = 450.f;
	//float fogEnd = 10000.f;

    FragColor = vec4((Diffuse), 1.0);


}