#pragma once
#include <vector>
class Distribution
{
public:
	Distribution(float mean, float var, int width);
	~Distribution();
	float generate(float x);
	float getMean();
	float getStd();
	int getWidth();
	std::vector<float> getCDT(int samples);
private:
	float mean, var;
	int width;
};

