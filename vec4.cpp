#include "vec4.h"

vec4::vec4()
{
	this->x = 0.0;
	this->y = 0.0;
	this->z = 0.0;
	this->w = 0.0;
}

vec4::vec4(double x, double y, double z, double w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

vec4 vec4::add(vec4& other)
{
	return vec4(this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w);
}

vec4 vec4::subtract(vec4& other)
{
	return vec4(this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w);
}

vec4 vec4::multiply(double scalar)
{
	return vec4(this->x * scalar, this->y * scalar, this->z * scalar, this->w * scalar);
}

double vec4::dot_product(vec4& other)
{
	return this->x * other.x + this->y * other.y + this->z * other.z + this->w * other.w;
}

double vec4::norm()
{
	return sqrt(x * x + y * y + z * z + w * w);
}

vec4 vec4::normalize()
{
	double n = norm();
	vec4 res = *this;

	if (n > e)
	{
		res = res * (1.0 / n);
	}

	return res;
}

vec4 vec4::operator+(vec4& other)
{
	return add(other);
}

vec4 vec4::operator-(vec4& other)
{
	return subtract(other);
}

double vec4::operator*(vec4& other)
{
	return dot_product(other);
}

vec4 vec4::operator*(double scalar)
{
	return multiply(scalar);
}

double& vec4::operator[](int ind)
{
	if (ind < 0 || ind >= size)
	{
		throw out_of_range("index is out of range.");
	}

	return components[ind];
}

vec4& vec4::operator=(vec4& other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	this->w = other.w;

	return *this;
}