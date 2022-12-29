in vec2 v_position;  // Vertex position
in vec2 v_texCoord;  // Vertex texture coordinate

out vec2 TexCoord;  // Varying variable for the fragment shader
out vec2 vPos;

void main() {
  gl_Position = vec4(v_position, 0.0, 1.0);  // Set the vertex position

  TexCoord = v_texCoord;  // Pass the texture coordinate to the fragment shader
  vPos = v_position;
}