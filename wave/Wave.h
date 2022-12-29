#pragma once

#include "../utils/Fourier.h"
#include "../Definitions.h"

namespace m2 {
	class Wave
	{
	public:
		Wave(int precision = 0);

		void FillTexture();
		void SetFourier(Fourier f);

		GLuint GetText();

	private:
		bool bufferOk;
		bool textOk;
		bool shaderOK;

		int precision;
		int width, heigh;

		GLuint frameBuffer;
		void LoadFramebuffer();

		GLuint texture;
		void LoadText();
		

		std::unique_ptr<Shader> shader;
		void LoadShader();

		Fourier generator;
	};

	std::shared_ptr<Wave> SlowUpFastDownWave(int precision = TEXT_DEF_PRECISION);

	std::shared_ptr<Wave> WavePreset(int preset, int precision = TEXT_DEF_PRECISION);
}