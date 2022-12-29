#version 330

in vec2 TexCoord;  // Texture coordinate of the fragment, passed from the vertex shader
in vec2 vPos;
in vec4 color;

out vec4 fragColor;  // Output color of the fragment

void main() {
  fragColor = color;// + vec4(0.5,0.5,0.5,1);
}