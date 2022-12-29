#version 330

in vec2 TexCoord;  // Texture coordinate of the fragment, passed from the vertex shader
in vec2 vPos;

out vec4 fragColor;  // Output color of the fragment

// Uniforms for frequency modulation
uniform float frequencies[20];
uniform float amplitudes_re[20];
uniform float amplitudes_im[20];
uniform vec3 scale;
uniform int prec;

void main() {
  // Calculate the sine of the x and y texture coordinates
  float T = scale.z;
  float S_re = scale.x;
  float S_im = scale.y;

  float sum_re = 0, sum_im = 0;

  for (int i = 0 ; i < prec ; i++) {
	sum_re += (cos(frequencies[i] * TexCoord.y * T) * amplitudes_re[i] - sin(frequencies[i] * TexCoord.y * T) * amplitudes_im[i]);// / S_re;
	sum_im += (cos(frequencies[i] * TexCoord.y * T) * amplitudes_im[i] + sin(frequencies[i] * TexCoord.y * T) * amplitudes_re[i]);// / S_im;
  }

  sum_re /= S_re;
  sum_im /= S_im;
  fragColor = vec4(0.5 + sum_im / 2.0, 0.5 + sum_re / 2.0,0,1);
}