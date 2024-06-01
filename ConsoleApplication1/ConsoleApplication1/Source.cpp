#include <iostream>
#include "schrodinger.h"
#include <complex>
using namespace std;

schrodinger s(2, 1, 1, 0);
#define PI 3.14159265358979323846  

int main()
{
	printf("hello world\n");
	printf("%f\n", s.laguerrePoly(2, 1, 6));
	assert(s.laguerrePoly(2, 1, 6) == 3.0, "First wrong !");
	

	auto res = s.sphericalHarmonics(2, 1, PI / 2, PI / 2);
	printf("%f %f\n", res.imag(), res.real());

	res = s.sphericalHarmonics(3, 1, PI / 2, PI / 3);
	printf("%f %f\n", res.imag(), res.real());
	return 0;
}