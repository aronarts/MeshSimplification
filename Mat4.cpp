#include "Mat4.h"
#include "Vector4.h"
#include <cassert>

Mat4::Mat4()
{
	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 4; ++j)
			value[i][j] = 0;
}

void Mat4::reset()
{
	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 4; ++j)
			value[i][j] = 0;
}

Mat4 operator +(Mat4& m1, Mat4& m2)
{
	Mat4 toret;
	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 4; ++j)
			toret.value[i][j] = m1.value[i][j] + m2.value[i][j];
	return toret;
}

Mat4 operator -(Mat4& m1, Mat4& m2)
{
	Mat4 toret;
	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 4; ++j)
			toret.value[i][j] = m1.value[i][j] - m2.value[i][j];
	return toret;
}

Mat4 operator *(Mat4& m1, Mat4& m2)
{
	Mat4 toret;
	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 4; ++j)
			for(int c = 0; c < 4; ++c)
					toret.value[i][j] += m1.value[i][c] * m2.value[c][j];
	return toret;
}
Mat4 multiplyToMat(Vector4& v1, Vector4& v2)
{
	Mat4 toret;
	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 4; ++j)
			toret.value[i][j] = v1.value[i] * v2.value[j];
	return toret;
}
