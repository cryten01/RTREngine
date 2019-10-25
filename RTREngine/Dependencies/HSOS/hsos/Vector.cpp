/*
 * Vector.cpp
 *
 *  Created on: 30.09.2014
 *      Author: fjalir
 */

#include "HSOS/Vector.h"
#include <cassert>
#include <math.h>
#include <iostream>
#define EPSILON 1e-5f //e-6 funktioniert nicht mehr..


Vector::Vector(const SimpleVector& vec) 
{
	std::copy(vec.mComponents, vec.mComponents + 3, mComponents);
}

Vector::Vector(const Vector& vec) : x(vec.x), y(vec.y), z(vec.z)
{
}

Vector::Vector(float x, float y, float z) :
X(x), Y(y), Z(z) {
}

Vector::Vector() : X(0), Y(0), Z(0){}

float Vector::dot(const Vector& v) const {
	return X * v.X + Y * v.Y + Z * v.Z;
}

Vector Vector::cross(const Vector& v) const {
	return Vector(Y * v.Z - Z * v.Y, Z * v.X - X * v.Z, X * v.Y - Y * v.X);
}

Vector Vector::operator +(const Vector& v) const {
	return Vector(X + v.X, Y + v.Y, Z + v.Z);
}

Vector Vector::operator -(const Vector& v) const {
	return Vector(X - v.X, Y - v.Y, Z - v.Z);
}

Vector& Vector::operator +=(const Vector& v) {
	X += v.X;
	Y += v.Y;
	Z += v.Z;
	return *this;
}

Vector Vector::operator *(float c) const {
	return Vector(X * c, Y * c, Z * c);
}

Vector Vector::operator -() const {
	return Vector(-X, -Y, -Z);
}

std::ostream& operator<<(std::ostream& strm, const Vector& v) {
		/*std::stringstream ss;
        ss << "(" <<  v.X << "," << v.Y << "," << v.Z << ")";
		return strm << ss.str().c_str();*/
		strm << '(' <<  v.X << "," << v.Y << "," << v.Z << ")";
		return strm;
	}

Vector& Vector::normalize() {
	float lengthSqrd = this->lengthSquared();
	if (lengthSqrd == 1.0f || lengthSqrd == 0.0f) return *this;
	//vllt hier rsqrt / inverserSquareRoot
	float length = 1.0f / sqrtf(lengthSqrd);
	X *= length;
	Y *= length;
	Z *= length;
	return *this;
}

Vector Vector::normalized() const {
	float lengthSqrd = this->lengthSquared();
	if (lengthSqrd == 1.0f || lengthSqrd == 0.0f) return *this;
	//vllt hier rsqrt / inverserSquareRoot
	float length = 1.0f / sqrtf(lengthSqrd);
	return Vector(X * length, Y * length, Z * length);

}

float Vector::length() const {
	return sqrtf(lengthSquared());
}

float Vector::lengthSquared() const {
	return X * X + Y * Y + Z * Z;
}

bool Vector::isZeroLength() const
{
	return lengthSquared() < EPSILON;
}

