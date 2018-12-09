#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

float FXAA_SPAN_MAX = 8.0f;
float FXAA_REDUCE_MUL = 1.0f/8.0f;
float FXAA_REDUCE_MIN = 1.0f/128.0f;

uniform sampler2D hdrBuffer;
uniform bool UseBlur;
uniform float exposure;
uniform vec2 hdrBufferOffset;

vec3 computeFxaa()
{
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

vec3 blur() {
	int blurSize = 8;
	vec2 texelSize = 1.0 / vec2(textureSize(hdrBuffer, 0));
	vec3 result = vec3(0, 0, 0);
	vec2 startOffset = vec2(float(1 - blurSize) / 2.0);
	for (int i = 0; i < blurSize; ++i) {
		for (int j = 0; j < blurSize; ++j) {
			vec2 offset = (startOffset + vec2(float(i), float(j))) * texelSize;
			result += texture2D(hdrBuffer, TexCoords + offset).rgb;
		}
	}
	result = result / float(blurSize * blurSize);
	return result;
}

float A = 0.15;
float B = 0.50;
float C = 0.10;
float D = 0.20;
float E = 0.02;
float F = 0.30;
float W = 11.2;

vec3 FilmicToneMapping(vec3 x) {
	return ((x*(A*x + C * B) + D * E) / (x*(A*x + B) + D * F)) - E / F;
}

void main()
{
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
		const float gamma = 2.2;
		vec3 color = computeFxaa();

		//color *= 8;

		color *= 2.9;
		float exBias = 2.0f;
		vec3 curr = FilmicToneMapping(exBias*color);
		vec3 whiteScale = 1.0f/FilmicToneMapping(vec3(W));
		curr *= whiteScale;

		curr = pow(curr, vec3(1.0 / gamma));
		if (UseBlur) curr += blur();

		FragColor = vec4(curr, 1.0);
}