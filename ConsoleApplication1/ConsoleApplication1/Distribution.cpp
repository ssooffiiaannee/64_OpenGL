#include <assert.h>
#include "Distribution.h"
#include <math.h>

Distribution::Distribution(float mean, float var, int width): mean(mean), var(var), width(width)
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

int Distribution::getWidth()
{
	return width;
}

std::vector<float> Distribution::getCDT(int samples)
{
	std::vector<float> v;
	float s = 0;
	v.push_back(s);
	for (size_t i = 1; i < samples + 1; i++)
	{
		s += generate(i * 1.0 / samples * width * sqrt(var) - width/2.0 * sqrt(var)) / samples * width * sqrt(var);
		v.push_back(s);
	}
	assert(1 - v.back() < 0.01, "CDT did not reach 1.0");
	v.push_back(1.0);
	return v;
}
