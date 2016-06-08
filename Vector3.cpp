#include "Vector3.h"
#include <cmath>
#include <iostream>
using namespace std;

Vector3::Vector3()
{
	x = 0; y = 0; z = 0;
}

Vector3::Vector3(double _x, double _y, double _z)
{
	x = _x; y = _y; z = _z;
}

void Vector3::normalize()//attention to accuracy
{
	double sum = sqrt(x * x + y * y + z * z);
	if(sum > 1e-10)
	{
		x = x / sum;
		y = y / sum;
		z = z / sum;
	}
	else
	{
		cout << "here" << endl;
		if(abs(x) > abs(y) && abs(x) > abs(z))
		{
			x = 1; y = 0; z = 0;
		}
		else if(abs(y) > abs(z) && abs(y) > abs(x))
		{
			x = 0; y = 1; z = 0;
		}
		else
		{
			x = 0; y = 0; z = 1;
		}
	}
}

Vector3 Vector3::operator=(Vector3& _v)
{
	x = _v.x; y = _v.y; z = _v.z;
	return *this;
}

Vector3 operator +(Vector3& v1, Vector3& v2)
{
	Vector3 toret;
	toret.x = v1.x + v2.x;
	toret.y = v1.y + v2.y;
	toret.z = v1.z + v2.z;
	return toret;
}
Vector3 operator -(Vector3& v1, Vector3& v2)
{
	Vector3 toret;
	toret.x = v1.x - v2.x;
	toret.y = v1.y - v2.y;
	toret.z = v1.z - v2.z;
	return toret;
}
double operator *(Vector3& v1, Vector3& v2)
{
	double toret = 0;
	toret += v1.x * v2.x;
	toret += v1.y * v2.y;
	toret += v1.z * v2.z;
	return toret;
}

Vector3 cross(Vector3& v1, Vector3& v2)
{
	Vector3 toret;
	toret.x = v1.y * v2.z - v1.z * v2.y;
	toret.y = v1.z * v2.x - v1.x * v2.z;
	toret.z = v1.x * v2.y - v1.y * v2.x;
	return toret;
}
