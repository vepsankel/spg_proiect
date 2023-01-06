#version 330

#define MAX_WAVES 10

in vec2 v_texCoord;
in vec2 v_position;  // Vertex position

out vec4 color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float time;

uniform int wave_num;
uniform sampler2D texs[MAX_WAVES];
uniform mat3 texCoordPositions[MAX_WAVES];
uniform mat3 texCoordSpeeds[MAX_WAVES];

void main() {
	vec3 texCoord = vec3(0,0,0);
	float height = 0;

	vec4 modelPos = Model * vec4(v_position.x, 0, v_position.y, 1); 
	vec2 modelPosV2 = vec2(modelPos.x, modelPos.z);

	int i;
	mat3 texCoordPosition, texCoordSpeed;
	for (i = 0 ; i < wave_num ; i++) {
		texCoordPosition = texCoordPositions[i];
		texCoordSpeed = texCoordSpeeds[i];

		//vec3 texCoordV3 = texCoordPosition * vec3(v_texCoord, 1) + time * texCoordSpeed * vec3(v_texCoord,1);
		vec3 texCoordV3 = texCoordPosition * vec3(modelPosV2, 1) + time * texCoordSpeed * vec3(modelPosV2,1);
		vec2 texCoordV2 = vec2(texCoordV3.x, texCoordV3.y);

		vec4 heightTexColor = texture(texs[i], vec2(texCoordV2.x, texCoordV2.y));

		float dh = heightTexColor.y - 0.5;
	
		if (i == 0) height += dh * 15; else height += dh / 2;

		if (i == 0) color = vec4(0,0,0.3 + 0.7*dh,1); else color += vec4(0,0,0.1*dh,1); // TODO
	}

	gl_Position = Projection * View * Model * vec4(v_position.x, 0.1 * height, v_position.y, 1.0);  // Set the vertex position
}