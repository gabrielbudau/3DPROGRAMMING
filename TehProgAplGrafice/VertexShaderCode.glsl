#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 vertexColor;

uniform vec3 ambientLight;

out vec3 theColor;
uniform mat4 fulltransformMatrix;

void main()
{
	vec4 v = vec4(position, 1.0);
	gl_Position = fulltransformMatrix * v;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	theColor = vertexColor * ambientLight;
}
