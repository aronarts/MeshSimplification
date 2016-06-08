#ifndef FACE_H
#define FACE_H


struct Face
{
	int p[3];

	Face();
	Face(int v1, int v2, int v3);
	void pointSort();
};

bool operator ==(Face& f1, Face& f2);

#endif