#include "vec4.h"

vec4::vec4()
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
	this->w = 0.0f;
}

vec4::vec4(float x, float y, float z, float w)
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

vec4 vec4::multiply(float scalar)
{
	return vec4(this->x * scalar, this->y * scalar, this->z * scalar, this->w * scalar);
}

float vec4::dot_product(vec4& other)
{
	return this->x * other.x + this->y * other.y + this->z * other.z + this->w * other.w;
}

float vec4::norm()
{
	return sqrt(x * x + y * y + z * z + w * w);
}

vec4 vec4::normalize()
{
	float n = norm();
	vec4 res = *this;

	if (n > e)
	{
		res = res * (1.0f / n);
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

float vec4::operator*(vec4& other)
{
	return dot_product(other);
}

vec4 vec4::operator*(float scalar)
{
	return multiply(scalar);
}

float& vec4::operator[](int ind)
{
	if (ind < 0 || ind >= size)
	{
		throw out_of_range("index is out of range.");
	}

	return raw[ind];
}

vec4& vec4::operator=(vec4& other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	this->w = other.w;

	return *this;
}