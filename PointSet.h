#ifndef POINTSET_H
#define POINTSET_H

#include "Point.h"

struct Point;
struct PointSet//start from 1
{
	Point* point;
	int count;
	bool* enabled;
	int maxpos;

	PointSet();
	int addPoint(Point& _p);
	void calculateErrorMat();
	void delPoint(int pos);
};
#endif