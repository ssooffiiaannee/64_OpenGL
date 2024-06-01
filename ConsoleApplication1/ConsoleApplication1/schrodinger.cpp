#include "schrodinger.h"
#include <cmath>
#include <boost/math/special_functions/laguerre.hpp>
#include <boost/math/special_functions/spherical_harmonic.hpp>
#include <complex>
using namespace boost::math::policies;
//
// Define a policy that sets ::errno on overflow, and does
// not promote double to long double internally:
//
typedef policy<domain_error<errno_on_error>, promote_double<true> > mypolicy;
mypolicy pol;

schrodinger::schrodinger(int n, int m, int l, int samples): n(n), l(l), m(m), samples(samples)
{
	assert(n > 0, "n has to be integer positive !");
	assert(l >= 0 && l < n, "l has to be non negative integer less that n !");
	assert(m >= -l && m <= l, "m has to be between -l and l !");
}

schrodinger::~schrodinger()
{
}

double schrodinger::constPart()
{
	return std::pow(2 / n, 3) * fact(n - l - 1) / (2 * n * fact(n + l));
}

void schrodinger::setRange(double r)
{
	range = r;
}

int schrodinger::fact(int n)
{
	if (n == 0) return 1;
	return n * fact(n - 1);
}

double schrodinger::laguerrePoly(int n, int m, double x)
{
	return boost::math::laguerre<double>(n, m, x);
}

double schrodinger::radiusVar(double r)
{
	return std::exp(-r / n) * std::pow(2 * r / n, l);
}

double schrodinger::schroLaguerre(double x)
{
	return laguerrePoly(n - l - 1, 2 * l + 1, x);
}

std::complex<double> schrodinger::sphericalHarmonics(int l, int m, double theta, double phi)
{
	return boost::math::spherical_harmonic<double, double>(l, m, theta, phi);
}

double schrodinger::fullPDF(double r, double theta, double phi)
{
	return radiusVar(r) * constPart() * schroLaguerre(r * 2 / n) * sphericalHarmonics(l, m, theta, phi).real();
}


