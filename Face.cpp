#include "Face.h"
#include <algorithm>
using namespace std;
Face::Face()
{
	for(int i = 0; i < 3; ++i)
		p[i] = 0;
}

Face::Face(int v1, int v2, int v3)
{
	p[0] = v1;
	p[1] = v2;
	p[2] = v3;
}

void Face::pointSort()
{
	sort(p, p + 3);
}

bool operator==(Face& f1, Face& f2)
{
	return (f1.p[0] == f2.p[0]) && (f1.p[1] == f2.p[1]) && (f1.p[2] == f2.p[2]);
}
