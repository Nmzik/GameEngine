#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

float FXAA_SPAN_MAX = 8.0f;
float FXAA_REDUCE_MUL = 1.0f/8.0f;
float FXAA_REDUCE_MIN = 1.0f/128.0f;

uniform sampler2D hdrBuffer;
uniform bool hdr;
uniform float exposure;

vec3 computeFxaa()
{
    vec2 hdrBufferOffset = vec2(1.0f/1280, 1.0f/720);
    vec3 luma = vec3(0.299f, 0.587f, 0.114f);

    vec3 offsetNW = texture(hdrBuffer, TexCoords.xy + (vec2(-1.0f, -1.0f) * hdrBufferOffset)).xyz;
    vec3 offsetNE = texture(hdrBuffer, TexCoords.xy + (vec2(1.0f, -1.0f) * hdrBufferOffset)).xyz;
    vec3 offsetSW = texture(hdrBuffer, TexCoords.xy + (vec2(-1.0f, 1.0f) * hdrBufferOffset)).xyz;
    vec3 offsetSE = texture(hdrBuffer, TexCoords.xy + (vec2(1.0f, 1.0f) * hdrBufferOffset)).xyz;
    vec3 offsetM  = texture(hdrBuffer, TexCoords.xy).xyz;

    float lumaNW = dot(luma, offsetNW);
    float lumaNE = dot(luma, offsetNE);
    float lumaSW = dot(luma, offsetSW);
    float lumaSE = dot(luma, offsetSE);
    float lumaM  = dot(luma, offsetNW);

    vec2 dir = vec2(-((lumaNW + lumaNE) - (lumaSW + lumaSE)),
                     ((lumaNW + lumaSW) - (lumaNE + lumaSE)));

    float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (FXAA_REDUCE_MUL * 0.25f), FXAA_REDUCE_MIN);
    float dirCorrection = 1.0f / (min(abs(dir.x), abs(dir.y)) + dirReduce);

    dir = min(vec2(FXAA_SPAN_MAX), max(vec2(-FXAA_SPAN_MAX), dir * dirCorrection)) * hdrBufferOffset;

    vec3 resultA = 0.5f * (texture(hdrBuffer, TexCoords.xy + (dir * vec2(1.0f / 3.0f - 0.5f))).xyz +
                                    texture(hdrBuffer, TexCoords.xy + (dir * vec2(2.0f / 3.0f - 0.5f))).xyz);

    vec3 resultB = resultA * 0.5f + 0.25f * (texture(hdrBuffer, TexCoords.xy + (dir * vec2(0.0f / 3.0f - 0.5f))).xyz +
                                             texture(hdrBuffer, TexCoords.xy + (dir * vec2(3.0f / 3.0f - 0.5f))).xyz);

    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
    float lumaResultB = dot(luma, resultB);

    if(lumaResultB < lumaMin || lumaResultB > lumaMax)
        return vec3(resultA);
    else
        return vec3(resultB);
}

void main()
{
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
    if(hdr)
    {
		vec3 result = computeFxaa();
		///FragColor = vec4(result, 1.0);

		vec3 mapped = vec3(1.0) - exp(-result * exposure);
		FragColor = vec4(mapped, 1.0);
    }
    else
    {
        vec3 result = pow(hdrColor, vec3(1.0));
        FragColor = vec4(result, 1.0);
    }
}