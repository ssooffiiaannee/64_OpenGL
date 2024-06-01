#pragma once
#include <assert.h>
#include <complex>
class schrodinger
{
public:
	schrodinger(int n, int m, int l, int samples);
	~schrodinger();
	double constPart();
	void setRange(double r);
	int fact(int n);
	double laguerrePoly(int n, int m, double x);
	double radiusVar(double r);
	double schroLaguerre(double x);
	std::complex<double> sphericalHarmonics(int l, int m, double theta, double phi);
	double fullPDF(double r, double theta, double phi);
private:
	int n, m, l;
	int samples;
	double range = 5;
};

