#include "vec3.h"

vec3::vec3()
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
}

vec3::vec3(float x, float y, float z)
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

vec3 vec3::multiply(float scalar)
{
	return vec3(this->x * scalar, this->y * scalar, this->z * scalar);
}

float vec3::dot_product(vec3& other)
{
	return this->x * other.x + this->y * other.y + this->z * other.z;
}

vec3 vec3::cross_product(vec3& other)
{
	return vec3(this->y * other.z - other.y * this->z, -(this->x * other.z - other.x * this->z), this->x * other.y - other.x * this->y);
}

float vec3::norm()
{
	return sqrt(x * x + y * y + z * z);
}

vec3 vec3::normalize()
{
	float n = norm();
	vec3 res = *this;

	if (n > e)
	{
		res = res * (1.0f / n);
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

float vec3::operator*(vec3& other)
{
	return dot_product(other);
}

vec3 vec3::operator*(float scalar)
{
	return multiply(scalar);
}

vec3 vec3::operator^(vec3& other)
{
	return cross_product(other);
}

float& vec3::operator[](int ind)
{
	if (ind < 0 || ind >= size)
	{
		throw out_of_range("index is out of range.");
	}

	return raw[ind];
}

vec3& vec3::operator=(vec3& other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;

	return *this;
}