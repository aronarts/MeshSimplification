#include "PointSet.h"
#include "Point.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Mat4.h"
#include <iostream>

PointSet::PointSet()
{
	count = 0;
	point = new Point[400000];
	enabled = new bool[400000];
	maxpos = 1;
	for(int i = 0; i < 400000; ++i)
		enabled[i] = true;
	enabled[0] = false;
}

int PointSet::addPoint(Point& _p)
{
	count++;
	point[maxpos] = _p;
	return maxpos++;
}

void PointSet::calculateErrorMat()
{
	for(int p = 1; p <= count; ++p)
		point[p].calculateMat(this);
}

void PointSet::delPoint(int pos)
{
	enabled[pos] = false;
	count--;
}
