#version 330

in vec2 v_texCoord;
in vec2 v_position;  // Vertex position

out vec4 color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float time;

uniform sampler2D tex;
uniform mat3 texCoordPosition;
uniform mat3 texCoordSpeed;

void main() {
	vec3 texCoordV3 = texCoordPosition * vec3(v_texCoord, 1);
	texCoordV3 += time * texCoordSpeed * vec3(1,1,1);
	vec2 texCoord = vec2(texCoordV3.x, texCoordV3.y);

	vec4 texColor = texture(tex, vec2(texCoord.x, texCoord.y));

	gl_Position = Projection * View * Model * vec4(v_position.x, texColor.y - 0.5, v_position.y, 1.0);  // Set the vertex position
	color = texColor;
}