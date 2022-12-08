#version 330 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 in_Color;
layout(location = 2) in vec3 vNormal;

//uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec3 out_Color;

void main()
{
	//gl_Position = transform * vec4(vPos, 1.0f);
	gl_Position = projection * view * model * vec4(vPos, 1.0f);

	FragPos = vec3(model * vec4(vPos,1.0));
	Normal = vNormal;
	out_Color = in_Color;
}