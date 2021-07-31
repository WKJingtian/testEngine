#type vertex
#version 430

layout(location = 0) in vec3 laPos;
layout(location = 1) in vec2 laCord;
layout(location = 2) in vec4 laColor;
layout(location = 3) in float laTex;
layout(location = 4) in vec3 laNormal;

uniform mat4 u_MVP = mat4(1);

out vec2 v_TCord;
out vec4 v_color;
out float v_tex;
out vec3 v_Normal;

void main()
{
	v_TCord = laCord;
	v_color = laColor;
	v_tex = laTex;
	v_Normal = laNormal;
	gl_Position = u_MVP * vec4(laPos, 1.0);
}

#type fragment
#version 430

layout(location = 0) out vec4 color;

uniform sampler2D uTexture[32];

in vec2 v_TCord;
in vec4 v_color;
in float v_tex;
in vec3 v_Normal;

void main()
{
	color = texture2D(uTexture[int(v_tex)], v_TCord) * v_color;
}
