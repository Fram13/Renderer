#include "vec3.h"

vec3::vec3()
{
	this->x = 0.0;
	this->y = 0.0;
	this->z = 0.0;
}

vec3::vec3(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

vec3 vec3::add(vec3& other)
{
	return vec3(this->x + other.x, this->y + other.y, this->z + other.z);
}

vec3 vec3::subtract(vec3& other)
{
	return vec3(this->x - other.x, this->y - other.y, this->z - other.z);
}

vec3 vec3::multiply(double scalar)
{
	return vec3(this->x * scalar, this->y * scalar, this->z * scalar);
}

double vec3::dot_product(vec3& other)
{
	return this->x * other.x + this->y * other.y + this->z * other.z;
}

vec3 vec3::cross_product(vec3& other)
{
	return vec3(this->y * other.z - other.y * this->z, -(this->x * other.z - other.x * this->z), this->x * other.y - other.x * this->y);
}

double vec3::norm()
{
	return sqrt(x * x + y * y + z * z);
}

vec3 vec3::normalize()
{
	double n = norm();
	vec3 res = *this;

	if (n > e)
	{
		res = res * (1.0 / n);
	}
	
	return res;
}

vec3 vec3::operator+(vec3& other)
{
	return add(other);
}

vec3 vec3::operator-(vec3& other)
{
	return subtract(other);
}

double vec3::operator*(vec3& other)
{
	return dot_product(other);
}

vec3 vec3::operator*(double scalar)
{
	return multiply(scalar);
}

vec3 vec3::operator^(vec3& other)
{
	return cross_product(other);
}

double& vec3::operator[](int ind)
{
	if (ind < 0 || ind >= size)
	{
		throw out_of_range("index is out of range.");
	}

	return components[ind];
}

vec3& vec3::operator=(vec3& other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;

	return *this;
}