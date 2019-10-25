/*
 * Vector.h
 *
 *  Created on: 30.09.2014
 *      Author: fjalir
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#include <string>

struct SimpleVector
{
	union
	{
		float mComponents[3];
		struct { float X, Y, Z; };
		struct { float x, y, z; };
	};
};

class Vector {
public:
	union
	{
		float mComponents[3];
		struct{ float X, Y, Z; };
		struct{ float x, y, z; };
		SimpleVector simple;
	};

	Vector(const SimpleVector& vec); // nicht explicit, da gewollter conversion-constructor
	Vector(const Vector& vec);
	Vector(float x, float y, float z);
	Vector();
	virtual ~Vector(){};

	float dot(const Vector& v) const;
	Vector cross(const Vector& v) const;
	Vector operator+(const Vector& v) const;
	Vector operator-(const Vector& v) const;
	Vector& operator+=(const Vector& v);
	Vector operator*(float c) const;
	// * -1
	Vector operator-() const;
	Vector& normalize();
	Vector normalized() const;
	float length() const;
	float lengthSquared() const;
	bool isZeroLength() const;
	
	
	friend std::ostream& operator<<(std::ostream& strm, const Vector& v);
private:
};

#endif /* VECTOR_H_ */
