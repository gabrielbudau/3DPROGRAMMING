#version 430

out vec4 daColor;
in vec3 theColor;

void main()
{
	gl_FragColor = texture2D( colorMap, gl_TexCoord[0].st);
	daColor = vec4(theColor, 1.0);
}