#version 120

// this is coming from our C++ code
uniform float mouseX;
uniform float timer;
uniform sampler2DRect tex0;

varying vec2 texCoord;
vec4 vertex;

void main()
{
    texCoord = gl_MultiTexCoord0.xy;

    vec4 position = ftransform();
    vec4 modifiedPosition = position;

	gl_Position = modifiedPosition;
}
