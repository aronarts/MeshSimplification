#ifndef PAIR_H
#define PAIR_H

#include "Point.h"
#include "PointSet.h"

struct Pair
{
	int v1;
	int v2;
	Point bestPoint;
	double delCost;
	
	Pair();
	Pair(int _v1, int _v2);
	void sort();
	void calculateBestPoint(PointSet* set);
	void calculateDelCost(PointSet* set);
	bool isFeaturePair(PointSet* set);
	Pair operator =(Pair& p);
};

#endif