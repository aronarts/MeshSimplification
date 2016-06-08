#ifndef POINT_H
#define POINT_H

#include "Vector3.h"
#include <vector>
#include "Mat4.h"
#include "PointSet.h"

using namespace std;

struct PointSet;
struct Point
{
	Vector3 cdt;
	vector<int> neighbor;
	Mat4 error;

	Point();
	Point(Vector3 _cdt);
	Point operator= (Point& _p);
	bool hasNeighbor(int neiId);
	void addNeighbor(int neiId);
	void calculateMat(PointSet* set);
	void accelerateFinding(PointSet* set);
};

#endif