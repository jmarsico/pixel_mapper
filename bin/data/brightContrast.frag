uniform float brightness;
uniform float contrast;
uniform float alpha;
uniform sampler2DRect tex0;

varying vec2 texCoord;

void main() {

	vec3 color = texture2DRect(tex0, texCoord).rgb;
	color = (((color - 0.5) * max(contrast,0.0)) + 0.5) * brightness;
	// vec3 colorContrasted = (color) * contrast;
	// vec3 bright = colorContrasted + vec3(brightness,brightness,brightness);
	gl_FragColor = vec4(color, 1.0);

}