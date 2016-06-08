#include "Point.h"
#include "Vector3.h"
#include "Vector4.h"
#include <iostream>
using namespace std;
Point::Point()
{
	cdt.x = 0;
	cdt.y = 0;
	cdt.z = 0;
}
Point::Point(Vector3 _cdt)
{
	cdt = _cdt;
}

bool Point::hasNeighbor(int neiId)
{
	vector<int>::iterator iter;
	for(iter = neighbor.begin(); iter != neighbor.end(); ++iter)
		if(*iter == neiId)
			return true;
	return false;
}

void Point::addNeighbor(int neiId)
{
	if(!hasNeighbor(neiId))
		neighbor.push_back(neiId);
}

void Point::calculateMat(PointSet* set)
{
	int isize = (int)neighbor.size();
	error.reset();
	for(int i = 0; i < isize; ++i)
	{
		for(int j = i + 1; j < isize; ++j)
			if(set->point[neighbor[i]].hasNeighbor(neighbor[j]))
			{
				Vector3 norm = cross(set->point[neighbor[i]].cdt - cdt, set->point[neighbor[j]].cdt - cdt);
				norm.normalize();
				double d = - (cdt * norm);
				Vector4 l(norm, d);
				error = error + l.makeMat(l);//can do += optimize
			}
	}
}

void Point::accelerateFinding(PointSet* set)
{
	int nsize = neighbor.size();
	for(int i = 0; i < nsize - 1; ++i)
	{
		int j;
		for(j = i + 1; j < nsize; ++j)
			if(set->point[neighbor[i]].hasNeighbor(neighbor[j]))
			{
				int tmp = neighbor[i + 1];
				neighbor[i + 1] = neighbor[j];
				neighbor[j] = tmp;
				break;
			}
	}
}

Point Point::operator=(Point& _p)
{
	cdt = _p.cdt;
	error = _p.error;//might have a bug?
	neighbor = _p.neighbor;
	return *this;
}
