#include "Distribution.h"
#include <math.h>

Distribution::Distribution(float mean, float var): mean(mean), var(var)
{
}

Distribution::~Distribution()
{
}

float Distribution::generate(float x)
{
	return 1 / (sqrt(var * 2 * 3.14159)) * expf(-(0.5) * pow((x - mean) / sqrt(var), 2));
}

float Distribution::getMean()
{
	return mean;
}

float Distribution::getStd()
{
	return sqrt(var);
}

std::vector<float> Distribution::getCDT(int samples)
{
	std::vector<float> v;
	float s = 0;
	v.push_back(s);
	for (size_t i = 1; i < samples + 1; i++)
	{
		s += generate(i * 1.0 / samples) / samples;
		v.push_back(s);
	}

	return v;
}
