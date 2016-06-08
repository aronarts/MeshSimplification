#include <iostream>
#include <cstdlib>
#include "Simplification.h"

using namespace std;

int main(int argc, char** argv)
{
	if(argc != 4)
	{
		cout << "Parameters error." << endl;
		cout << "Usage: MeshSimplification [inputPath] [outputPath] [ratio]" << endl;
		return 1;
	}
	Simplification s;
	s.setInput(string(argv[1]));
	s.setOutput(string(argv[2]));
	s.setRatio(atof(argv[3]));
	s.exacute();
	return 0;
}