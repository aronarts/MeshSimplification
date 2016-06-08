#include "Pair.h"
#include "Vector4.h"
#include <cmath>

#include <iostream>
using namespace std;
Pair::Pair()
{
	v1 = 0;
	v2 = 0;
	delCost = 0;
}

Pair::Pair(int _v1, int _v2)
{
	v1 = _v1;
	v2 = _v2;
	delCost = 0;
}

void Pair::sort()
{
	if(v1 > v2)
	{
		int tmp = v1;
		v1 = v2;
		v2 = tmp;
	}
}

void Pair::calculateBestPoint(PointSet* set)
{
	
	bestPoint.cdt = set->point[v1].cdt + set->point[v2].cdt;
	bestPoint.cdt.x /= 2;
	bestPoint.cdt.y /= 2;
	bestPoint.cdt.z /= 2;
	//直接取中点
	//linear optimization采用高斯消元法
	Mat4 data = set->point[v1].error + set->point[v2].error;
	data.value[3][0] = 0;
	data.value[3][1] = 0;
	data.value[3][2] = 0;
	data.value[3][3] = 1;
	Vector4 Y(Vector3(0, 0, 0), 1);
	for(int i = 0; i < 4; ++i)
		data.value[i][3] *= -1;
	for(int i = 0; i < 3; ++i)//对于每个方程
	{
		int j;
		for(j = 0; j < 3; ++j)//对于方程左边的系数
			if(abs(data.value[i][j]) >= 1e-6)//检测到有不为0的系数
				break;
		
		if(j == 3)//如果系数均为0
			return;
		for(int p = 0; p < 3; ++p)//对于非原方程的每个方程
			if(p != i)
			{
				double d = data.value[p][j] / data.value[i][j];
				for(int k = 0; k < 4; ++k)//对于每个参数
					data.value[p][k] -= data.value[i][k] * d;
			}
	}
	for(int i = 0; i < 3; ++i)
	{
		int count = 0;
		for(int j = 0; j < 3; ++j)
			if(abs(data.value[i][j]) < 1e-6) count++;
		if(count == 3) return;
	}
	double index[3];
	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 3; ++j)
			if(abs(data.value[i][j]) > 1e-6)
				index[j] = data.value[i][3] / data.value[i][j];
	bestPoint.cdt.x = index[0];
	bestPoint.cdt.y = index[1];
	bestPoint.cdt.z = index[2];
	
}
void Pair::calculateDelCost(PointSet* set)
{

	Vector4 v(bestPoint.cdt, 1.0);
	Mat4 Q = set->point[v1].error + set->point[v2].error;
	Vector4 vQ;
	for(int i = 0; i < 4; ++i)
	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 4; ++j)
			vQ.value[i] += v.value[j] * Q.value[j][i];
	delCost = vQ * v;
}

bool Pair::isFeaturePair(PointSet* set)
{
	int count = 0;
	for(int i = 0; i < (int)set->point[v1].neighbor.size(); ++i)
		if(set->point[v2].hasNeighbor(set->point[v1].neighbor[i]))
			count++;
	if(count != 2)
		return true;
	else return false;
}

Pair Pair::operator=(Pair& p)
{
	v1 = p.v1;
	v2 = p.v2;
	bestPoint = p.bestPoint;
	delCost = p.delCost;
	return *this;
}
