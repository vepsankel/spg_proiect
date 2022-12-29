#version 330

in vec2 v_position;  // Vertex position
in vec2 v_texCoord;  // Vertex texture coordinate

out vec2 TexCoord;  // Varying variable for the fragment shader
out vec2 vPos;
out vec4 color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float time;
uniform sampler2D tex;

void main() {
  vec4 texColor = texture(tex, vec2(v_texCoord.x, v_texCoord.y - time / 10));

  gl_Position = Projection * View * Model * vec4(v_position.x, texColor.y - 0.5, v_position.y, 1.0);  // Set the vertex position

  TexCoord = v_texCoord;  // Pass the texture coordinate to the fragment shader
  vPos = v_position;
  color = texColor;
}