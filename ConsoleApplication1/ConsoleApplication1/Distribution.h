#pragma once
#include <vector>
class Distribution
{
public:
	Distribution(float mean, float var);
	~Distribution();
	float generate(float x);
	float getMean();
	float getStd();
	std::vector<float> getCDT(int samples);
private:
	float mean, var;
};

