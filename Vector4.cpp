#include "Vector4.h"
#include "Mat4.h"
#include <cassert>

Vector4::Vector4()
{
	for(int i = 0; i < 4; ++i)
		value[i] = 0;
}

Vector4::Vector4(Vector3& v, double w)
{
	value[0] = v.x;
	value[1] = v.y;
	value[2] = v.z;
	value[3] = w;
}

Mat4 Vector4::makeMat(Vector4& v)
{
	Mat4 toret;
	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 4; ++j)
			toret.value[i][j] = value[i] * v.value[j];
	return toret;
}

double& Vector4::operator[](int pos)
{
	assert(pos >= 0 && pos < 4);
	return value[pos];
}

double operator*(Vector4& v1, Vector4& v2)
{
	double toret = 0;
	for(int i = 0; i < 4; ++i)
		toret += v1.value[i] * v2.value[i];
	return toret;
}
