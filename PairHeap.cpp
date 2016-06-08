#include "PairHeap.h"
#include <map>
#include <iostream>
#include <fstream>
using namespace std;
PairHeap::PairHeap()
{
	count = 0;
}

void PairHeap::setupHeap(PointSet* set)
{
	cout << "SETUP HEAP\n" << endl;
	ofstream fout("mypair.txt");
	for(int i = 1; i <= set->count; ++i)
		for(int j = 0; j < (int)set->point[i].neighbor.size(); ++j)
		{
			int k = set->point[i].neighbor[j];
			if(i < k)
			{
				Pair toAdd(i, k);
				toAdd.calculateBestPoint(set);
				toAdd.calculateDelCost(set);
				//cout << "ADD:(" << i << "," << k << ") delCost:" << toAdd.delCost << endl;
				fout << i << " " << k << " " << toAdd.delCost << endl;
				addPair(&toAdd);
			}
		}
}

void PairHeap::addPair(Pair* p)
{
	count++;
	pairQueue.push(*p);
	mapper[make_pair(p->v1, p->v2)] = true;
}

void PairHeap::deletePair(Pair* p)
{
	count--;
	mapper[make_pair(p->v1, p->v2)] = false;
}

Pair PairHeap::top()
{
	while(!pairQueue.empty())
	{
		Pair toret = pairQueue.top();
		pairQueue.pop();
		if(mapper[make_pair(toret.v1, toret.v2)] == true)
			return toret;
	}
	Pair empty(-1, -1);
	return empty;
}

bool PairHeap::cmp::operator()(Pair& p1, Pair& p2)
{
	return p1.delCost > p2.delCost;
}
