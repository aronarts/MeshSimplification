#ifndef VECTOR4_H
#define VECTOR4_H

#include "Vector3.h"
#include "Mat4.h"

struct Vector4
{
	double value[4];
	Vector4();
	Vector4(Vector3& v, double w);
	double& operator [](int pos);
	Mat4 makeMat(Vector4& v);
};

double operator *(Vector4& v1, Vector4& v2);
#endif