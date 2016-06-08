#ifndef VECTOR3_H
#define VECTOR3_H

struct Vector3
{
	double x, y, z;

	Vector3();
	Vector3(double _x, double _y, double _z);
	void normalize();
	Vector3 operator= (Vector3& _v);

};

Vector3 operator +(Vector3& v1, Vector3& v2);
Vector3 operator -(Vector3& v1, Vector3& v2);
double operator *(Vector3& v1, Vector3& v2);
Vector3 cross(Vector3& v1, Vector3& v2);

#endif