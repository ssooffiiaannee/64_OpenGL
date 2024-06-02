#pragma once
#include <assert.h>
#include <complex>
class schrodinger
{
public:
	schrodinger(int n, int l, int m, double r = 10);
	~schrodinger();
	double constPart();
	void setRange(double r);
	double getRange() const;
	double getThetaRange() const;
	double getPhiRange() const;
	double getConstPart() const;
	int fact(int n);
	double laguerrePoly(int n, int m, double x);
	double radiusVar(double r);
	double schroLaguerre(double x);
	std::complex<double> sphericalHarmonics(int l, int m, double theta, double phi);
	double generate(double r, double theta, double phi);
private:
	int n, m, l;
	int samples;
	double range = 5;
	double constP;
};

