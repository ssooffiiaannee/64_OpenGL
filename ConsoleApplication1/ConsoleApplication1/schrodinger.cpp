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

#define PI	 (double) 3.14159265358979323846

schrodinger::schrodinger(int n, int l, int m, double r): n(n), l(l), m(m), range(r)
{
	assert(n > 0, "n has to be integer positive !");
	assert(l >= 0 && l < n, "l has to be non negative integer less that n !");
	assert(m >= -l && m <= l, "m has to be between -l and l !");

	constP = constPart();
}

schrodinger::~schrodinger()
{
}

double schrodinger::constPart()
{
#ifdef _DEBUG
	//printf("(double)fact(n - l - 1) / double(2 * n * fact(n + l)) = %1.10f   std::pow(2 / n, 3) = %f\n", (double)fact(n - l - 1) / double(2 * n * fact(n + l)), std::pow((double) 2.0 / n, 3));
#endif
	return std::pow((double)2.0 / n, 3) * (double) fact(n - l - 1) / double (2 * n * fact(n + l));
}

void schrodinger::setRange(double r)
{
	range = r;
}

double schrodinger::getRange() const
{
	return range;
}

double schrodinger::getThetaRange() const
{
	return 2 * PI;
}

double schrodinger::getPhiRange() const
{
	return PI;
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

double schrodinger::getConstPart() const
{
	return constP;
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

double schrodinger::generate(double r, double theta, double phi)
{
#ifdef _DEBUG
	/*printf("radiusVar(r) = %1.10f   r = %f\n", radiusVar(r), r);
	printf("constPart() = %1.10f \n", constPart());
	printf("schroLaguerre(r * 2 / n) = %1.10f \n", schroLaguerre(r * 2 / n));
	printf("std::abs(sphericalHarmonics(l, m, theta, phi)) = %1.10f \n", std::abs(sphericalHarmonics(l, m, theta, phi)));*/
#endif

	return radiusVar(r) * getConstPart() * schroLaguerre(r * 2 / n) * abs(sphericalHarmonics(l, m, theta, phi).real());
}


