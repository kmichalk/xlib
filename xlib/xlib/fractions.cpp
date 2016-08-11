#include "fractions.h"

Frac::Frac():
	num{0}, denom{0}
{
}

Frac::Frac(int num, int denom):
	num{num}, denom{denom}
{
}

Frac::Frac(Frac const & other):
	num{other.num}, denom{other.denom}
{
}

Frac::Frac(int value):
	nun{value}, denom{1}
{
}

Frac::Frac(double value)
{
}

Frac::Frac(float value)
{
}

Frac & Frac::operator=(Frac const & other)
{
	num = other.num;
	denom = other.denom;
}

bool Frac::operator==(Frac const & other)
{
	return num*other.denom == other.num*denom;
}

Frac Frac::operator+(Frac const & other) const
{
	return Frac{num*other.denom+other.num*denom, denom*other.denom};
}
