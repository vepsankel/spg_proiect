#include "Fourier.h"

using namespace std;

m2::Fourier::Fourier()
{
}

void m2::Fourier::SetSample(std::vector<std::complex<float>> sample)
{
	this->sample = sample;

	this->amps.resize(GetPeriod());
	this->ang_freqs.resize(GetPeriod());

	Calculate();
}

void m2::Fourier::Calculate()
{
	std::vector<float> freqs_local;
	int sample_size;

	sample_size = GetPeriod();

	for (int k = 0; k < sample_size; k++) {
		ang_freqs[k] = M_PI * 2.f / (float)sample_size * k;
	}

	for (int k = 0; k < sample_size; k++) {
		std::complex<float> sum = 0;

		for (int x = 0; x < sample_size; x++) {
			sum += sample[x] * std::complex<float>(cos(ang_freqs[k] * x), -sin(ang_freqs[k] * x));
		}

		amps[k] = sum;
	}

	Sort();
}

void m2::Fourier::Sort() {
	float temp_fl;
	std::complex<float> temp_cm;

	for (int k = 0 ; k < GetPeriod() ; k++)
	for (int i = 0; i < GetPeriod() - 1; i++) {
		if (std::abs(ang_freqs[i]) > std::abs(ang_freqs[i + 1])) {
			temp_cm = amps[i];
			temp_fl = ang_freqs[i];

			amps[i] = amps[i + 1];
			ang_freqs[i] = ang_freqs[i + 1];

			amps[i + 1] = temp_cm;
			ang_freqs[i + 1] = temp_fl;
		}
	}
}

const int m2::Fourier::GetPeriod() const
{
	return sample.size();
}

const std::vector<std::complex<float>> m2::Fourier::GetSample() const
{
	return sample;
}

const std::vector<float> m2::Fourier::GetAngFreqs() const
{
	return ang_freqs;
}

const std::vector<std::complex<float>> m2::Fourier::GetAmps() const
{
	return amps;
}

const std::vector<float> m2::Fourier::GetAmpsRe() const
{
	std::vector<float> ampsRe;
	ampsRe.resize(amps.size());

	for (int i = 0; i < amps.size(); i++)
		ampsRe[i] = amps[i].real();

	return ampsRe;
}

const std::vector<float> m2::Fourier::GetAmpsIm() const
{
	std::vector<float> ampsIm;
	ampsIm.resize(amps.size());

	for (int i = 0; i < amps.size(); i++)
		ampsIm[i] = amps[i].imag();

	return ampsIm;
}

glm::vec3 m2::Fourier::GetScale(int iterations, int precision)
{
	// ret values
	float maxAmpRe = 0, maxAmpIm = 0;

	float x;
	std::complex<float> sum;
	for (int i = 0; i < iterations; i++) {
		x = (float)GetPeriod() * (float)i / (float)iterations;
		sum = 0;

		for (int f = 0; f < precision; f++) {
			sum += amps[f] * std::complex<float>(cos(ang_freqs[f] * x), sin(ang_freqs[f] * x));
		}

		maxAmpRe = (maxAmpRe < abs(sum.real())) ? abs(sum.real()) : maxAmpRe;
		maxAmpIm = (maxAmpIm < abs(sum.imag())) ? abs(sum.imag()) : maxAmpIm;
	}

	/*
	for (int i = 0; i < GetPeriod(); i++) {
		//maxAmpRe = (maxAmpRe < abs(sample[i].real())) ? abs(sample[i].real()) : maxAmpRe;
		//maxAmpIm = (maxAmpIm < abs(sample[i].imag())) ? abs(sample[i].imag()) : maxAmpIm;

		maxAmpRe = (maxAmpRe < abs(amps[i].real())) ? abs(amps[i].real()) : maxAmpRe;
		maxAmpIm = (maxAmpIm < abs(amps[i].imag())) ? abs(amps[i].imag()) : maxAmpIm;
	}
	*/
	return glm::vec3(maxAmpRe, maxAmpIm, GetPeriod());
}

std::ostream& m2::operator<<(std::ostream& os, const Fourier& f)
{
	os << "T=" << f.GetPeriod() << " [";
	for (int i = 0; i < f.GetPeriod(); i++)
		os << f.GetSample()[i] << " ";
	os << "]" << endl;
	os << "AngFreq\t|\tAmp" << endl;
	for (int i = 0; i < f.GetPeriod(); i++)
		os << f.GetAngFreqs()[i] << "\t|\t" << f.GetAmps()[i] << endl;

	return os;
}
