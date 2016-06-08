#ifndef MAT4_H
#define MAT4_H


struct Mat4
{
	double value[4][4];

	Mat4();
	void reset();
};

Mat4 operator +(Mat4& m1, Mat4& m2);
Mat4 operator -(Mat4& m1, Mat4& m2);
Mat4 operator *(Mat4& m1, Mat4& m2);

#endif