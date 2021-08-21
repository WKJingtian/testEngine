#type vertex
#version 430 core
layout(location = 0) in vec3 laPos;
layout(location = 1) in vec2 laCord;
layout(location = 2) in vec4 laColor;
layout(location = 3) in float laTex;

uniform mat4 uViewProject = mat4(1);

out vec2 v_TCord;
out vec4 v_color;
out float v_tex;

void main()
{
	v_TCord = laCord;
	v_color = laColor;
	v_tex = laTex;
	gl_Position = uViewProject * vec4(laPos, 1);
}

#type fragment
#version 430 core
layout(location = 0) out vec4 color;
uniform sampler2D uTexture[32];

in vec2 v_TCord;
in vec4 v_color;
in float v_tex;
			
void main()
{
	color = texture2D(uTexture[int(v_tex)], v_TCord) * v_color;
}