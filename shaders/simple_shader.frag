#version 450

layout(location=0) in vec3 fragColor;
layout(location=1) in vec3 fragPosWorld;
layout(location=2) in vec3 fragNormalWorld;

layout(location=0) out vec4 outColor;

layout(set = 0, binding = 0) uniform GlobalUbo {
	mat4 projection;
	mat4 view;
	vec4 ambientLightColor;
	vec3 lightPosition;
	vec4 lightColor;
} ubo;

layout(push_constant) uniform Push{
	mat4 modelMatrix;  //projection * view * model
	mat4 normalMatrix;
} push;

void main()
{
	vec3 directionToLight = ubo.lightPosition - fragPosWorld;
	float attenuation = 1.0/dot(directionToLight, directionToLight);

	vec3 lightColor = ubo.lightColor.xyz * ubo.lightColor.w * attenuation;
	vec3 ambientLightColor = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;
	vec3 diffuseLight = lightColor * max(dot(normalize(fragNormalWorld), normalize(directionToLight)), 0);

	vec3 color = (diffuseLight + ambientLightColor) * fragColor;
	outColor = vec4(color, 1.0);
}