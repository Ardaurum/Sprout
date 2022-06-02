#ifdef __SHADER_TYPE_VERTEX__
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_UV;

layout (std140, binding = 0) uniform uPerFrame
{
	mat4 u_ViewProjection;
	float u_Time;
};

layout (std140, binding = 1) uniform uPerObject
{
	uniform mat4 u_Model;
};

out vec2 v_UV;

void main()
{
	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
	v_UV = a_UV;
}		
#endif

#ifdef __SHADER_TYPE_FRAGMENT__
layout(location = 0) out vec4 out_color;

layout (std140, binding = 0) uniform uPerFrame
{
	mat4 u_ViewProjection;
	float u_Time;
};

uniform sampler2D u_Main;

in vec2 v_UV;

void main()
{
	out_color = texture(u_Main, v_UV);
}		
#endif