#pragma once

#include "../Definitions.h"

namespace m2 {


	class Fourier
	{
	public:
		Fourier();

		void SetSample(std::vector<std::complex<float>> sample);
		void Calculate();

		const int GetPeriod() const;
		const std::vector<std::complex<float>> GetSample() const;
		const std::vector<float> GetAngFreqs() const;
		const std::vector<std::complex<float>> GetAmps() const;
		const std::vector<float> GetAmpsRe() const;
		const std::vector<float> GetAmpsIm() const;

		glm::vec3 GetScale(int iterations, int precision);

	private:
		std::vector<std::complex<float>> sample;
		std::vector<float> ang_freqs;
		std::vector<std::complex<float>> amps;

		void Sort();
	};

	std::ostream& operator<<(std::ostream& os, const Fourier& f);
}