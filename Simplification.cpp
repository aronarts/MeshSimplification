#include "Simplification.h"
#include <fstream>
#include <iostream>
#include "Vector3.h"
#include "Point.h"
#include <set>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include "Face.h"

using namespace std;

bool Simplification::readFile()
{
	FILE *filein = fopen(inputFile.c_str(), "r");

	if(filein == NULL)
	{
		cout << "Can't open the input file." << endl;
		return false;
	}
	char info;
	char cmtLine[1000];

	while(fscanf(filein, "%c", &info) != EOF)
	{
		if(info == 'v')
		{
			Point newPoint;
			fscanf(filein, "%lf%lf%lf", &newPoint.cdt.x, &newPoint.cdt.y, &newPoint.cdt.z);
			pointSet.addPoint(newPoint);
		}
		else if(info == 'f')
		{
			int index[3];
			fscanf(filein, "%d%d%d", &index[0], &index[1], &index[2]);
			for(int j = 0; j < 3; ++j)
				for(int k = j + 1; k < 3; ++k)
					if(!pointSet.point[index[j]].hasNeighbor(index[k]))
					{
						pointSet.point[index[j]].addNeighbor(index[k]);
						pointSet.point[index[k]].addNeighbor(index[j]);
					}
			faceCount++;
		}
		else if(info == '#')
			fgets(cmtLine, 1000, filein);
	}
	fclose(filein);
	return true;
}

void Simplification::updateData(Pair toDel)
{
	pairHeap.deletePair(&toDel);//删除此边
	int np = pointSet.addPoint(toDel.bestPoint);//加入新点
	set<int> neighbors;
	for(int i = 0; i < (int)pointSet.point[toDel.v1].neighbor.size(); ++i)
	{
		int k = pointSet.point[toDel.v1].neighbor[i];
		if(k != toDel.v2)
			neighbors.insert(k);
	}
	for(int i = 0; i < (int)pointSet.point[toDel.v2].neighbor.size(); ++i)
	{
		int k = pointSet.point[toDel.v2].neighbor[i];
		if(k != toDel.v1)
			neighbors.insert(k);
	}//建立v1,v2邻居集
	set<int>::iterator nb;
	for(nb = neighbors.begin(); nb != neighbors.end(); ++nb)
	{
		pointSet.point[np].neighbor.push_back(*nb);
	}//更新新点的邻居集

	for(nb = neighbors.begin(); nb != neighbors.end(); ++nb)
	{
		vector<int>::iterator iter;
		for(iter = pointSet.point[*nb].neighbor.begin(); iter != pointSet.point[*nb].neighbor.end();)
		{
			if(*iter == toDel.v1 || *iter == toDel.v2)
				iter = pointSet.point[*nb].neighbor.erase(iter);
			else
				++iter;
		}
		pointSet.point[*nb].addNeighbor(np);
		pointSet.point[*nb].calculateMat(&pointSet);
	}//对邻居集的每一个点删除原先的点v1,v2并加入新点到邻居中，计算误差矩阵
	pointSet.point[np].calculateMat(&pointSet);
	//maybe a mistake?
	for(nb = neighbors.begin(); nb != neighbors.end(); ++nb)
	{
		Pair p1 = Pair(*nb, toDel.v1);
		Pair p2 = Pair(*nb, toDel.v2);
		p1.sort();
		p2.sort();
		map<pair<int, int>, bool>::iterator iter;
		iter = pairHeap.mapper.find(make_pair(p1.v1, p1.v2));
		if(iter != pairHeap.mapper.end())
		{
			if(pairHeap.mapper[make_pair(p1.v1, p1.v2)] == true)
				pairHeap.deletePair(&Pair(p1.v1, p1.v2));
		}
		iter = pairHeap.mapper.find(make_pair(p2.v1, p2.v2));
		if(iter != pairHeap.mapper.end())
		{
			if(pairHeap.mapper[make_pair(p2.v1, p2.v2)] == true)
				pairHeap.deletePair(&Pair(p2.v1, p2.v2));
		}
		Pair toAdd = Pair(*nb, np);
		toAdd.sort();
		toAdd.calculateBestPoint(&pointSet);
		toAdd.calculateDelCost(&pointSet);
		pairHeap.addPair(&toAdd);

	}//删除旧的关联边，加入新的关联边
	pointSet.delPoint(toDel.v1);
	pointSet.point[toDel.v1].neighbor.clear();
	pointSet.delPoint(toDel.v2);
	pointSet.point[toDel.v2].neighbor.clear();
}

void Simplification::saveFile()
{
	FILE *fileout = fopen(outputFile.c_str(), "w");
	
	vector<int> pointOut;//输出点集
	map<int, int> outMapper;//点集映射
	int pos = 0;
	for(int outCount = 0; outCount < pointSet.count;)
	{
		if(pointSet.enabled[pos])//如果是可用点则加入输出点集
		{
			pointOut.push_back(pos);
			outMapper[pos] = outCount;//建立逆映射
			outCount++;
		}
		pos++;
	}
	for(int i = 0; i < (int)pointOut.size(); ++i)
		fprintf(fileout, "v %.8lf %.8lf %.8lf\n", pointSet.point[pointOut[i]].cdt.x, 
				pointSet.point[pointOut[i]].cdt.y, pointSet.point[pointOut[i]].cdt.z);
	
	
	struct cmp
	{
		bool operator ()(Face f1, Face f2)
		{
			if(f1.p[0] < f2.p[0]) return true;
			if(f1.p[0] > f2.p[0]) return false;
			if(f1.p[1] < f2.p[1]) return true;
			if(f1.p[1] > f2.p[1]) return false;
			if(f1.p[2] < f2.p[2]) return true;
			return false;
		}
	};
	set<Face, cmp> faceOut;
	for(int i = 0; i < (int)pointOut.size(); ++i)
	{
		int insize = (int)pointSet.point[pointOut[i]].neighbor.size();
		for(int v1 = 0; v1 < insize; ++v1)
		{
			for(int v2 = v1 + 1; v2 < insize; ++v2)
			{
				if(pointSet.point[pointSet.point[pointOut[i]].neighbor[v1]].hasNeighbor(
					pointSet.point[pointOut[i]].neighbor[v2]))
				{
					Point p = pointSet.point[pointOut[i]];
					Face toAdd(outMapper[pointOut[i]] + 1, outMapper[p.neighbor[v1]] + 1, outMapper[p.neighbor[v2]] + 1);
					toAdd.pointSort();
					faceOut.insert(toAdd);
				}
			}
		}
	}
	set<Face, cmp>::iterator iter;
	
	for(iter = faceOut.begin(); iter != faceOut.end(); ++iter)
	{
		fprintf(fileout, "f %d %d %d\n", (*iter).p[0], (*iter).p[1], (*iter).p[2]);
	}
	
	fclose(fileout);
}

Simplification::Simplification()
{
	inputFile = "";
	outputFile = "";
	ratio = 0.5;
	faceCount = 0;
}

void Simplification::setInput(string _input)
{
	inputFile = _input;
}

void Simplification::setOutput(string _output)
{
	outputFile = _output;
}

void Simplification::setRatio(double _ratio)
{
	ratio = _ratio;
}

void Simplification::exacute()
{
	time_t start = clock();
	readFile();
	pointSet.calculateErrorMat();
	pairHeap.setupHeap(&pointSet);
	time_t end = clock();
	
	cout << "initialization time cost: " << end - start << endl;
	start = end;
	
	int nowCount = faceCount;
	while((double)nowCount > (double)faceCount * ratio)
	{
		Pair toDel = pairHeap.top();
		updateData(toDel);	
		nowCount -= 2;
	}
	cout << "------END------\n";
	for(int i = 0; i < pointSet.maxpos; ++i)
	{
		/*if(pointSet.enabled[i])
		{
			cout << i << ":(" << pointSet.point[i].cdt.x << "," << pointSet.point[i].cdt.y 
				<< "," << pointSet.point[i].cdt.z << ")" << endl;
			cout << "list:";
			for(int j = 0; j < (int)pointSet.point[i].neighbor.size(); ++j)
				cout << pointSet.point[i].neighbor[j] << " ";
			cout << endl;
		}*/
		/*
		cout << i << " Matrix:" << endl;
		for(int j = 0; j < 4; ++j)
		{
			for(int k = 0; k < 4; ++k)
				cout << setprecision(8) << pointSet.point[i].error.value[j][k] << " ";
			cout << endl;
		}*/
	}
	saveFile();
	end = clock();
	cout << "calculating time cost: " << end - start << endl;
}

void Simplification::outMat()
{
	ofstream fileout("mymat.txt");
	for(int i = 1; i < pointSet.maxpos; ++i)
	{
		/*if(pointSet.enabled[i])
		{
			cout << i << ":(" << pointSet.point[i].cdt.x << "," << pointSet.point[i].cdt.y 
				<< "," << pointSet.point[i].cdt.z << ")" << endl;
			cout << "list:";
			for(int j = 0; j < (int)pointSet.point[i].neighbor.size(); ++j)
				cout << pointSet.point[i].neighbor[j] << " ";
			cout << endl;
		}*/
		
		for(int j = 0; j < 4; ++j)
		{
			for(int k = 0; k < 4; ++k)
				fileout << setprecision(8) << pointSet.point[i].error.value[j][k] << " ";
			fileout << endl;
		}
	}

}
