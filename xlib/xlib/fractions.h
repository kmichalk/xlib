#ifndef FRACTIONS_H
#define FRACTIONS_H

struct Frac
{
	int num, denom;

	Frac();
	Frac(int num, int denom);
	Frac(Frac const& other);
	Frac(int value);
	Frac(double value);
	Frac(float value);
	
	Frac& operator=(Frac const& other);
	bool operator==(Frac const& other);
	Frac operator+(Frac const& other) const;
	Frac& operator+=(Frac const& other);
	Frac operator-(Frac const& other);
	Frac& operator-=(Frac const& other) const;
	Frac operator*(Frac const& other);
	Frac& operator*=(Frac const& other) const;
	Frac operator/(Frac const& other);
	Frac& operator/=(Frac const& other) const;
	operator bool() const;
	operator double() const;
	operator long double() const;
	operator float() const;

};

#endif
